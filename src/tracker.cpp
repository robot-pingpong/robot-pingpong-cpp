#include "tracker.h"
#include "constants.h"
#include <sstream>

Tracker::Tracker(cv::Mat &screen, cv::viz::Viz3d &visualizer)
    : first(0), second(1) {
  capture(true);
  halfSize = cv::Size(firstFrame.cols / 2, firstFrame.rows);
  firstFrame.copyTo(screen);

  visualizer.showWidget("Coordinate Widget", cv::viz::WCoordinateSystem());
  auto table = cv::viz::WPlane(cv::Size2d(X_TABLE_SIZE, Y_TABLE_SIZE));
  table.setRenderingProperty(cv::viz::REPRESENTATION,
                             cv::viz::REPRESENTATION_WIREFRAME);
  visualizer.showWidget("table", table);
  auto net = cv::viz::WPlane(cv::Point3d(0, 0, 0.07625), cv::Point3d(0, -1, 0),
                             cv::Point3d(0, 0, 1),
                             cv::Size2d(X_TABLE_SIZE + 0.305, 0.1525));
  net.setRenderingProperty(cv::viz::REPRESENTATION,
                           cv::viz::REPRESENTATION_WIREFRAME);
  visualizer.showWidget("net", net);
  visualizer.setViewerPose(cv::viz::makeCameraPose(
      cv::Point3d(3, -10, 5), cv::Point3d(0, 0, 0), cv::Point3d(0, 0, -1)));
  visualizer.showWidget("ball", ball);
}

void Tracker::setMask() {
  auto data = cv::FileStorage("mask.yml", cv::FileStorage::READ);
  std::vector<cv::Point> firstMask, secondMask;
  if (data.isOpened()) {
    data["first"] >> firstMask;
    data["second"] >> secondMask;
    data.release();
  }
  data.open("mask.yml", cv::FileStorage::WRITE);
  cv::write(data, "first", first.setGlobalMask("first mask", firstMask));
  cv::write(data, "second", second.setGlobalMask("second mask", secondMask));
  data.release();
}

void Tracker::setTableArea() {
  auto data = cv::FileStorage("points.yml", cv::FileStorage::READ);
  std::vector<cv::Point2f> firstPoints, secondPoints;
  if (data.isOpened()) {
    data["first"] >> firstPoints;
    data["second"] >> secondPoints;
    data.release();
  }
  data.open("points.yml", cv::FileStorage::WRITE);
  firstPoints = first.getTableArea("first area", firstPoints);
  secondPoints = second.getTableArea("second area", secondPoints);
  cv::write(data, "first", firstPoints);
  cv::write(data, "second", secondPoints);
  data.release();

  std::vector<std::vector<cv::Point3f>> objectPoints = {
      {{0, Y_TABLE_SIZE, 0},
       {X_TABLE_SIZE, Y_TABLE_SIZE, 0},
       {X_TABLE_SIZE, 0, 0},
       {0, 0, 0},
       {X_TABLE_SIZE / 2, -0.1525, 0.15},
       {X_TABLE_SIZE / 2, Y_TABLE_SIZE + 0.1525, 0.15}}};

  std::vector<std::vector<cv::Point2f>> fs = {{
      {firstPoints[0].x, firstPoints[0].y},
      {firstPoints[1].x, firstPoints[1].y},
      {firstPoints[2].x, firstPoints[2].y},
      {firstPoints[3].x, firstPoints[3].y},
      {firstPoints[4].x, firstPoints[4].y},
      {firstPoints[5].x, firstPoints[5].y},
  }};
  std::vector<std::vector<cv::Point2f>> ss = {{
      {secondPoints[2].x, secondPoints[2].y},
      {secondPoints[3].x, secondPoints[3].y},
      {secondPoints[0].x, secondPoints[0].y},
      {secondPoints[1].x, secondPoints[1].y},
      {secondPoints[5].x, secondPoints[5].y},
      {secondPoints[4].x, secondPoints[4].y},
  }};
  auto fCameraMatrix =
      cv::initCameraMatrix2D(objectPoints, fs, firstFrame.size());
  cv::Mat fDistCoeffs, fRVect, fTVect;
  double fRms = cv::calibrateCamera(
      objectPoints, fs, firstFrame.size(), fCameraMatrix, fDistCoeffs, fRVect,
      fTVect,
      cv::CALIB_USE_INTRINSIC_GUESS | cv::CALIB_FIX_FOCAL_LENGTH |
          cv::CALIB_FIX_ASPECT_RATIO | cv::CALIB_FIX_PRINCIPAL_POINT);

  auto sCameraMatrix =
      cv::initCameraMatrix2D(objectPoints, ss, secondFrame.size());
  cv::Mat sDistCoeffs, sRVect, sTVect;
  double sRms = cv::calibrateCamera(
      objectPoints, ss, secondFrame.size(), sCameraMatrix, sDistCoeffs, sRVect,
      sTVect,
      cv::CALIB_USE_INTRINSIC_GUESS | cv::CALIB_FIX_FOCAL_LENGTH |
          cv::CALIB_FIX_ASPECT_RATIO | cv::CALIB_FIX_PRINCIPAL_POINT);

  std::cout << fRms << ", " << sRms << std::endl;
  cv::Mat R, T, E, F;
  cv::stereoCalibrate(objectPoints, fs, ss, fCameraMatrix, fDistCoeffs,
                      sCameraMatrix, sDistCoeffs, firstFrame.size(), R, T, E,
                      F);

  cv::Mat fR, sR, Q;
  cv::stereoRectify(fCameraMatrix, fDistCoeffs, sCameraMatrix, sDistCoeffs,
                    firstFrame.size(), R, T, fR, sR, firstProjectionMatrix,
                    secondProjectionMatrix, Q);
}

void Tracker::capture(const bool render) {
  first.captureFrame();
  second.captureFrame();
  if (render) {
    cv::Point2f p1, p2;
    first.render(firstFrame, p1);
    second.render(secondFrame, p2);
  }
}

void Tracker::render(const cv::Mat &screen, cv::viz::Viz3d &visualizer) {
  capture();
  std::vector<cv::Point2f> firstPoint(1);
  std::vector<cv::Point2f> secondPoint(1);
  const auto firstSuccess = first.render(firstFrame, firstPoint[0]);
  const auto secondSuccess = second.render(secondFrame, secondPoint[0]);

  cv::resize(firstFrame, firstFrame, halfSize);
  cv::resize(secondFrame, secondFrame, halfSize);

  if (firstSuccess) {
    cv::line(firstFrame, cv::Point(0, static_cast<int>(firstPoint[0].y)),
             cv::Point(firstFrame.cols, static_cast<int>(firstPoint[0].y)),
             cv::Scalar(0, 255, 0), 2);
    cv::line(firstFrame, cv::Point(static_cast<int>(firstPoint[0].x / 2), 0),
             cv::Point(static_cast<int>(firstPoint[0].x / 2), firstFrame.rows),
             cv::Scalar(0, 255, 0), 2);
  }
  if (secondSuccess) {
    cv::line(secondFrame, cv::Point(0, static_cast<int>(secondPoint[0].y)),
             cv::Point(secondFrame.cols, static_cast<int>(secondPoint[0].y)),
             cv::Scalar(0, 255, 0), 2);
    cv::line(
        secondFrame, cv::Point(static_cast<int>(secondPoint[0].x / 2), 0),
        cv::Point(static_cast<int>(secondPoint[0].x / 2), secondFrame.rows),
        cv::Scalar(0, 255, 0), 2);
  }
  firstFrame.copyTo(screen(cv::Rect(0, 0, halfSize.width, halfSize.height)));
  secondFrame.copyTo(
      screen(cv::Rect(halfSize.width, 0, halfSize.width, halfSize.height)));

  if (!firstSuccess || !secondSuccess) {
    return;
  }
  if (firstProjectionMatrix.empty() || secondProjectionMatrix.empty()) {
    return;
  }
  cv::Mat point3d;
  cv::triangulatePoints(firstProjectionMatrix, secondProjectionMatrix,
                        firstPoint, secondPoint, point3d);

  cv::Mat point3dNormalized;
  cv::convertPointsFromHomogeneous(point3d.t(), point3dNormalized);
  point3dNormalized = point3dNormalized.t();

  std::stringstream ss;
  ss << "X: " << point3dNormalized.at<float>(0)
     << ", Y: " << point3dNormalized.at<float>(1)
     << ", Z: " << point3dNormalized.at<float>(2);
  cv::putText(screen, ss.str(), cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX, 1,
              cv::Scalar(255, 255, 255));

  visualizer.setWidgetPose(
      "ball", cv::Affine3d(cv::Vec3d(),
                           cv::Vec3d(point3d.at<float>(0), point3d.at<float>(1),
                                     point3d.at<float>(2))));
}
