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
  visualizer.showWidget("table", table,
                        cv::Affine3d(cv::Vec3d(0, 0, M_PI / 2), cv::Vec3d()));
  auto net = cv::viz::WPlane(cv::Size2d(Y_TABLE_SIZE + 0.305, 0.1525));
  net.setRenderingProperty(cv::viz::REPRESENTATION,
                           cv::viz::REPRESENTATION_WIREFRAME);
  visualizer.showWidget(
      "net", net,
      cv::Affine3d(cv::Vec3d(M_PI / 2, 0, 0), cv::Vec3d(0, 0, 0.1525 / 2)));
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
  firstMask = first.setGlobalMask("first mask", firstMask);
  secondMask = second.setGlobalMask("second mask", secondMask);
  data.open("mask.yml", cv::FileStorage::WRITE);
  data << "first" << firstMask;
  data << "second" << secondMask;
  data.release();
}

void Tracker::setTableArea(cv::viz::Viz3d &visualizer) {
  auto data = cv::FileStorage("points.yml", cv::FileStorage::READ);
  std::vector<cv::Point2f> firstPoints, secondPoints;
  if (data.isOpened()) {
    data["first"] >> firstPoints;
    data["second"] >> secondPoints;
    data.release();
  }
  firstPoints = first.getTableArea("first area", firstPoints);
  secondPoints = second.getTableArea("second area", secondPoints);
  data.open("points.yml", cv::FileStorage::WRITE);
  data << "first" << firstPoints;
  data << "second" << secondPoints;
  data.release();

  const std::vector<std::vector<cv::Point3f>> objectPoints = {
      {{0, Y_TABLE_SIZE, 0},
       {X_TABLE_SIZE, Y_TABLE_SIZE, 0},
       {X_TABLE_SIZE, 0, 0},
       {0, 0, 0},
       {X_TABLE_SIZE / 2, -0.1525, 0.15},
       {X_TABLE_SIZE / 2, Y_TABLE_SIZE + 0.1525, 0.15}}};

  const std::vector<std::vector<cv::Point2f>> screenPoints[] = {
      {firstPoints},
      {{
          secondPoints[2],
          secondPoints[3],
          secondPoints[0],
          secondPoints[1],
          secondPoints[5],
          secondPoints[4],
      }}};

  cv::Mat distCoeffs[2], R[2];
  std::vector<cv::Mat> rVecs[2], tVecs[2];
  const cv::Size size = firstFrame.size();
  for (int i = 0; i < 2; ++i) {
    //     mean = np.mean(p2d, axis=1)
    // std = np.std(p2d, axis=1)
    // camera_mtx = np.array([
    //   [1 / std[0], 0, -mean[0] / std[0]],
    //   [0, 1 / std[1], -mean[1] / std[1]],
    //   [0, 0, 1],
    // ])

    cv::Mat mean, std;
    cv::Mat a(screenPoints[i][0]);
    cv::meanStdDev(a, mean, std);
    double initMatrix[3][3] = {
        {1.0 / std.at<double>(0), 0.0, -mean.at<double>(0) / std.at<double>(0)},
        {0.0, 1.0 / std.at<double>(1), -mean.at<double>(1) / std.at<double>(1)},
        {0.0, 0.0, 1.0},
    };
    cameraMatrix[i] = cv::Mat(3, 3, CV_64F, initMatrix);
    cv::Mat screenPoint(screenPoints[i][0]);
    screenPoint.convertTo(screenPoint, CV_64F);
    screenPoint = screenPoint.reshape(1);
    cv::hconcat(screenPoint, cv::Mat::ones(screenPoint.rows, 1, CV_64F),
                screenPoint);
    cameraMatrix[i] *= screenPoint.t();
    const double rms = cv::calibrateCamera(
        objectPoints, screenPoints[i], size, cameraMatrix[i], distCoeffs[i],
        rVecs[i], tVecs[i],
        cv::CALIB_USE_INTRINSIC_GUESS | cv::CALIB_ZERO_TANGENT_DIST |
            cv::CALIB_FIX_ASPECT_RATIO | cv::CALIB_FIX_PRINCIPAL_POINT |
            cv::CALIB_FIX_TAUX_TAUY | cv::CALIB_FIX_K1 | cv::CALIB_FIX_K2 |
            cv::CALIB_FIX_K3 | cv::CALIB_FIX_K4 | cv::CALIB_FIX_K5 |
            cv::CALIB_FIX_K6 | cv::CALIB_FIX_S1_S2_S3_S4 |
            cv::CALIB_FIX_INTRINSIC);
    std::cout << rms << std::endl;
    cv::Rodrigues(rVecs[i][0], R[i]);
    cv::Mat position = (-R[i].inv() * tVecs[i][0]);
    auto camera = cv::viz::WCameraPosition(cv::Matx33d(cameraMatrix[0]), 1,
                                           i == 0 ? cv::viz::Color::blue()
                                                  : cv::viz::Color::red());
    std::cout << position << std::endl;
    visualizer.showWidget("camera" + std::to_string(i), camera,
                          cv::Affine3d(R[i], position));
  }

  // cv::initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1,
  //                             projectionMatrix[0], size, CV_16SC2,
  //                             rmap[0][0], rmap[0][1]);
  // cv::initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2,
  //                             projectionMatrix[1], size, CV_16SC2,
  //                             rmap[1][0], rmap[1][1]);
}

void Tracker::capture(const bool render) {
  first.captureFrame();
  second.captureFrame();
  // if (!rmap[0][0].empty()) {
  //   cv::remap(first.frame, first.frame, rmap[0][0], rmap[0][1],
  //             cv::INTER_LINEAR);
  //   cv::remap(second.frame, second.frame, rmap[1][0], rmap[1][1],
  //             cv::INTER_LINEAR);
  // }
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
  if (projectionMatrix[0].empty() || projectionMatrix[1].empty()) {
    return;
  }
  cv::Mat point3d;
  cv::triangulatePoints(projectionMatrix[0], projectionMatrix[1], firstPoint,
                        secondPoint, point3d);

  cv::Mat point3dNormalized;
  cv::convertPointsFromHomogeneous(point3d.t(), point3dNormalized);
  point3dNormalized = point3dNormalized.t();
  // point3dNormalized = point3d;

  std::stringstream ss;
  ss << "X: " << point3dNormalized.at<float>(0)
     << ", Y: " << point3dNormalized.at<float>(1)
     << ", Z: " << point3dNormalized.at<float>(2);
  cv::putText(screen, ss.str(), cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX, 1,
              cv::Scalar(255, 255, 255));

  visualizer.setWidgetPose(
      "ball",
      cv::Affine3d(cv::Vec3d(), cv::Vec3d(point3dNormalized.at<float>(0),
                                          point3dNormalized.at<float>(1),
                                          point3dNormalized.at<float>(2))));
}
