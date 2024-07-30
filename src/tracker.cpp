#include "tracker.h"
#include "constants.h"
#include "dlt.h"

#include <sstream>

static const std::vector<std::vector<cv::Point3d>> objectPoints = {
    {{0, Y_TABLE_SIZE, 0},
     {X_TABLE_SIZE, Y_TABLE_SIZE, 0},
     {X_TABLE_SIZE, 0, 0},
     {0, 0, 0},
     {X_TABLE_SIZE / 2, -0.1525, 0.15},
     {X_TABLE_SIZE / 2, Y_TABLE_SIZE + 0.1525, 0.15}}};

Tracker::Tracker(cv::Mat &screen, cv::viz::Viz3d &visualizer)
    : first(0), second(1) {
  capture(true);
  halfSize = cv::Size(firstFrame.cols / 2, firstFrame.rows / 2);
  firstFrame.copyTo(screen);

  visualizer.showWidget("Coordinate Widget", cv::viz::WCoordinateSystem());
  auto table = cv::viz::WPlane(cv::Size2d(X_TABLE_SIZE, Y_TABLE_SIZE));

  for (int i = 0; i < 4; ++i) {
    auto tableLine = cv::viz::WLine(cv::Point3d(objectPoints[0][i]),
                                    cv::Point3d(objectPoints[0][(i + 1) % 4]));
    tableLine.setRenderingProperty(cv::viz::LINE_WIDTH, 2);
    visualizer.showWidget("table line" + std::to_string(i), tableLine);
  }

  auto net = cv::viz::WPlane(cv::Size2d(Y_TABLE_SIZE + 0.305, 0.1525));
  net.setRenderingProperty(cv::viz::REPRESENTATION,
                           cv::viz::REPRESENTATION_WIREFRAME);
  visualizer.showWidget(
      "net", net,
      cv::Affine3d(cv::Vec3d(), cv::Vec3d())
          .rotate(cv::Vec3d(M_PI / 2))
          .rotate(cv::Vec3d(0, 0, M_PI / 2))
          .translate(cv::Vec3d(X_TABLE_SIZE / 2, Y_TABLE_SIZE / 2, 0.07625)));
  visualizer.setViewerPose(cv::viz::makeCameraPose(
      cv::Point3d(6, -0.5, 1.5),
      cv::Point3d(X_TABLE_SIZE / 2, Y_TABLE_SIZE / 2, 0),
      cv::Point3d(0, 0, -1)));
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

  const std::vector<std::vector<cv::Point2d>> screenPoints[] = {
      {{
          firstPoints[0],
          firstPoints[1],
          firstPoints[2],
          firstPoints[3],
          firstPoints[4],
          firstPoints[5],
      }},
      {{
          secondPoints[2],
          secondPoints[3],
          secondPoints[0],
          secondPoints[1],
          secondPoints[5],
          secondPoints[4],
      }}};

  cv::Mat distCoeffs[2], R[2];
  cv::Mat rVecs[2], tVecs[2];
  for (int i = 0; i < 2; ++i) {
    DLT::pose(objectPoints[0], screenPoints[i][0], cameraMatrix[i], R[i],
              tVecs[i], projectionMatrix[i]);
    cv::Mat position = (R[i].inv() * tVecs[i]);
    auto camera = cv::viz::WCameraPosition(cv::Matx33d(cameraMatrix[i]), 1,
                                           i == 0 ? cv::viz::Color::blue()
                                                  : cv::viz::Color::red());
    visualizer.showWidget("camera" + std::to_string(i), camera,
                          cv::Affine3d(R[i], position));
  }
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

bool Tracker::render(const cv::Mat &screen, cv::viz::Viz3d &visualizer) {
  capture();
  std::vector<cv::Point2f> firstPoint(1);
  std::vector<cv::Point2f> secondPoint(1);
  const auto firstSuccess = first.render(firstFrame, firstPoint[0]);
  const auto secondSuccess = second.render(secondFrame, secondPoint[0]);

  cv::resize(firstFrame, firstFrame, halfSize);
  cv::resize(secondFrame, secondFrame, halfSize);

  if (firstSuccess) {
    cv::line(firstFrame, cv::Point(0, static_cast<int>(firstPoint[0].y / 2)),
             cv::Point(firstFrame.cols, static_cast<int>(firstPoint[0].y / 2)),
             cv::Scalar(0, 255, 0), 2);
    cv::line(firstFrame, cv::Point(static_cast<int>(firstPoint[0].x / 2), 0),
             cv::Point(static_cast<int>(firstPoint[0].x / 2), firstFrame.rows),
             cv::Scalar(0, 255, 0), 2);
  }
  if (secondSuccess) {
    cv::line(
        secondFrame, cv::Point(0, static_cast<int>(secondPoint[0].y / 2)),
        cv::Point(secondFrame.cols, static_cast<int>(secondPoint[0].y / 2)),
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
    return false;
  }
  if (projectionMatrix[0].empty() || projectionMatrix[1].empty()) {
    return false;
  }
  cv::Mat point3d;
  cv::triangulatePoints(projectionMatrix[0], projectionMatrix[1], firstPoint,
                        secondPoint, point3d);

  cv::Mat point3dNormalized;
  cv::convertPointsFromHomogeneous(point3d.t(), point3dNormalized);
  point3dNormalized = point3dNormalized.t();
  point3dNormalized.convertTo(point3dNormalized, CV_64F);
  pos = point3dNormalized.at<cv::Vec3d>();
  visualizer.setWidgetPose("ball", cv::Affine3d(cv::Vec3d(), pos));
  return true;
}
