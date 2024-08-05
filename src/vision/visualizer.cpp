#include "visualizer.h"
#include "../constants.h"

#define WINDOW_NAME "screen"

Visualizer::Visualizer()
    : screen(900, 1600, CV_8UC3), top(450, 800, CV_8UC3),
      right(450, 800, CV_8UC3), ballVisible(false),
      machinePosition(Y_TABLE_SIZE / 2), hasStopped(false),
      resizedScreen(450, 1600, CV_8UC3) {
  // visualizer.setViewerPose(cv::viz::makeCameraPose(
  //     cv::Point3d(6, -0.5, 1.5),
  //     cv::Point3d(X_TABLE_SIZE / 2, Y_TABLE_SIZE / 2, 0),
  //     cv::Point3d(0, 0, -1)));
  // visualizer.showWidget("ball", ball);
  // visualizer.showWidget(
  //     "machine", machine,
  //     cv::Affine3d(cv::Vec3d(), cv::Vec3d(X_TABLE_SIZE, Y_TABLE_SIZE / 2,
  //     0)));

  std::stringstream fileName;
  fileName << "output" << std::time(nullptr) << ".mkv";
  writer.open(fileName.str(), cv::VideoWriter::fourcc('X', '2', '6', '4'), 30,
              cv::Size(1280 * 2, 720 * 2));
}

bool Visualizer::stopped() const { return hasStopped; }

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

void Visualizer::render(const double fps) {
  screen(cv::Rect(0, 0, 1600, 450)) = resizedScreen;
  renderRight();
  screen(cv::Rect(0, 450, 800, 450)) = top;
  screen(cv::Rect(800, 450, 800, 450)) = right;

  if (ballVisible) {
    std::stringstream ss;
    ss << "X: " << ballPosition[0] << ", Y: " << ballPosition[1]
       << ", Z: " << ballPosition[2];
    cv::putText(screen, ss.str(), cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX,
                1, cv::Scalar(255, 255, 255));
  }

  cv::putText(screen, "FPS: " + std::to_string(fps), cv::Point(10, 30),
              cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
  cv::imshow(WINDOW_NAME, screen);
  if (cv::waitKey(1) == 27) {
    hasStopped = true;
  }

  ballVisible = false;
}
void Visualizer::setBallPosition(const cv::Vec3d &vec) {
  ballVisible = true;
  ballPosition = vec;
}

void Visualizer::setMachinePosition(const double y) { machinePosition = y; }
void Visualizer::setScreen(const cv::Mat &screen) {
  cv::resize(screen, resizedScreen, cv::Size(1600, 450));
}
Visualizer::~Visualizer() {
  writer.release();
  cv::destroyWindow(WINDOW_NAME);
}
