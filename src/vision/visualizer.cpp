#include "visualizer.h"
#include "../constants.h"

Visualizer::Visualizer() {
  screen = cv::Mat(720, 1280 * 2, CV_8UC3);
  top = cv::Mat(720, 1280, CV_8UC3);
  right = cv::Mat(720, 1280, CV_8UC3);
  ballVisible = false;
  ballPosition = cv::Vec3d();
  machinePosition = Y_TABLE_SIZE / 2;
  // visualizer.setViewerPose(cv::viz::makeCameraPose(
  //     cv::Point3d(6, -0.5, 1.5),
  //     cv::Point3d(X_TABLE_SIZE / 2, Y_TABLE_SIZE / 2, 0),
  //     cv::Point3d(0, 0, -1)));
  // visualizer.showWidget("ball", ball);
  // visualizer.showWidget(
  //     "machine", machine,
  //     cv::Affine3d(cv::Vec3d(), cv::Vec3d(X_TABLE_SIZE, Y_TABLE_SIZE / 2,
  //     0)));
}

void Visualizer::addCamera(const int index, const cv::Matx33d &matrix,
                           const cv::Mat &rotation, const cv::Mat &position) {
  cameras.emplace_back(cv::viz::WCameraPosition(matrix),
                       cv::Affine3d(rotation, position));
}

void Visualizer::renderTop() {
  top = cv::Scalar();
  cv::circle(top, convertToTop(cv::Vec3d()), 3, RED, -1);
  cv::rectangle(top, convertToTop(cv::Vec3d(0, 0, 0)),
                convertToTop(cv::Vec3d(X_TABLE_SIZE, Y_TABLE_SIZE, 0)), WHITE,
                1);
  cv::rectangle(
      top, convertToTop(cv::Vec3d(X_TABLE_SIZE / 2, -0.1525, 0)),
      convertToTop(cv::Vec3d(X_TABLE_SIZE / 2, Y_TABLE_SIZE + 0.1525, 0.15)),
      WHITE, 1);
  if (ballVisible) {
    cv::circle(top, convertToTop(ballPosition), 10, cv::Scalar(0, 0, 255), -1);
  }
  cv::circle(top, convertToTop(cv::Vec3d(X_TABLE_SIZE, machinePosition, 0)), 10,
             RED, -1);
}
void Visualizer::renderRight() {
  right = cv::Scalar();
  cv::circle(right, convertToRight(cv::Vec3d()), 3, RED, -1);
  cv::rectangle(right, convertToRight(cv::Vec3d(0, 0, 0)),
                convertToRight(cv::Vec3d(X_TABLE_SIZE, Y_TABLE_SIZE, 0)), WHITE,
                1);
  cv::rectangle(
      right, convertToRight(cv::Vec3d(X_TABLE_SIZE / 2, -0.1525, 0)),
      convertToRight(cv::Vec3d(X_TABLE_SIZE / 2, Y_TABLE_SIZE + 0.1525, 0.15)),
      WHITE, 1);
  if (ballVisible) {
    cv::circle(right, convertToRight(ballPosition), 10, RED, -1);
  }
}

cv::Point Visualizer::convertToTop(const cv::Vec3d &vec) {
  return {640 + static_cast<int>((vec[0] - X_TABLE_SIZE / 2) * 300),
          360 - static_cast<int>((vec[1] - Y_TABLE_SIZE / 2) * 300)};
}
cv::Point Visualizer::convertToRight(const cv::Vec3d &vec) {
  return {640 + static_cast<int>((vec[0] - X_TABLE_SIZE / 2) * 300),
          360 - static_cast<int>((vec[2] - 0.15) * 300)};
}

const cv::Mat &Visualizer::render() {
  renderTop();
  renderRight();
  cv::hconcat(top, right, screen);
  ballVisible = false;
  return screen;
}
void Visualizer::setBallPosition(const cv::Vec3d &vec) {
  ballVisible = true;
  ballPosition = vec;
}

void Visualizer::setMachinePosition(const double y) { machinePosition = y; }
