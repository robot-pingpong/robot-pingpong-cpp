#include "visualizer.h"
#include "../constants.h"

#define WINDOW_NAME "screen"

Visualizer::Visualizer(const Predictor &predictor)
    : screen(720 * 2, 1280 * 2, CV_8UC3), visionScreen(720, 1280 * 2, CV_8UC3),
      windowScreen(900, 1600, CV_8UC3), top(720, 1280, CV_8UC3),
      right(720, 1280, CV_8UC3), ballVisible(false),
      machinePosition(Y_TABLE_SIZE / 2), hasStopped(false),
      predictor(predictor) {
  std::stringstream fileName;
  fileName << "output" << std::time(nullptr) << ".mkv";
  writer.open(fileName.str(), cv::VideoWriter::fourcc('X', '2', '6', '4'), 30,
              cv::Size(1280 * 2, 720 * 2));
  cv::namedWindow(WINDOW_NAME);
}

bool Visualizer::stopped() const { return hasStopped; }

void Visualizer::addCamera(const int index, const cv::Matx33d &matrix,
                           const cv::Mat &rotation, const cv::Mat &position) {
  cameras.emplace_back(cv::viz::WCameraPosition(matrix),
                       cv::Affine3d(rotation, position));
}

void Visualizer::renderTopRight() {
  circle(cv::Vec3d(), 3, RED, 1);
  rect(cv::Vec3d(0, 0, 0), cv::Vec3d(X_TABLE_SIZE, Y_TABLE_SIZE, 0), WHITE);
  rect(cv::Vec3d(X_TABLE_SIZE / 2, -0.1525, 0),
       cv::Vec3d(X_TABLE_SIZE / 2, Y_TABLE_SIZE + 0.1525, 0.15), WHITE);
  if (ballVisible) {
    circle(ballPosition, 10, RED);
  }
  cv::circle(top, convertToTop(cv::Vec3d(X_TABLE_SIZE, machinePosition, 0)), 10,
             RED, -1);

  for (const auto &history : predictor.history) {
    circle(history, 5, GREEN, -1);
  }
  double y = 0, z = 0;
  if (!predictor.predictY(y))
    y = 0;
  if (!predictor.predictZ(z))
    z = 0;
  if (y != 0) {
    line(cv::Vec3d(0, y, 0), cv::Vec3d(X_TABLE_SIZE - 0.35, y, 0), GREEN, 2);
    line(cv::Vec3d(X_TABLE_SIZE - 0.35, y, 0),
         cv::Vec3d(X_TABLE_SIZE - 0.35, y, 4), GREEN, 2);
  }
  if (z != 0) {
    line(cv::Vec3d(X_TABLE_SIZE - 0.55, y, z),
         cv::Vec3d(X_TABLE_SIZE - 0.15, y, z), GREEN, 2);
  }
  if (!predictor.boundIndicies.empty()) {
    std::vector<cv::Point> points;
    double x = 0;
    while (x < X_TABLE_SIZE) {
      const auto height = predictor.boundQuadratic.at(0) +
                          predictor.boundQuadratic.at(1) * x +
                          predictor.boundQuadratic.at(2) * x * x;
      points.push_back(convertToRight(cv::Vec3d(x, 0, height)));
      x += 0.01;
    }
    cv::polylines(right, points, false, CYAN, 2);
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
void Visualizer::circle(const cv::Vec3d &center, const int radius,
                        const cv::Scalar &color, const int thickness) {
  cv::circle(top, convertToTop(center), radius, color, thickness);
  cv::circle(right, convertToRight(center), radius, color, thickness);
}
void Visualizer::line(const cv::Vec3d &start, const cv::Vec3d &end,
                      const cv::Scalar &color, const int thickness) {
  cv::line(top, convertToTop(start), convertToTop(end), color, thickness);
  cv::line(right, convertToRight(start), convertToRight(end), color, thickness);
}
void Visualizer::rect(const cv::Vec3d &pt1, const cv::Vec3d &pt2,
                      const cv::Scalar &color) {
  cv::rectangle(top, convertToTop(pt1), convertToTop(pt2), color, 1);
  cv::rectangle(right, convertToRight(pt1), convertToRight(pt2), color, 1);
}

void Visualizer::render(const double fps) {
  screen = cv::Scalar();
  visionScreen.copyTo(screen(cv::Rect(0, 0, 1280 * 2, 720)));
  top = cv::Scalar();
  right = cv::Scalar();

  renderTopRight();

  top.copyTo(screen(cv::Rect(0, 720, 1280, 720)));
  right.copyTo(screen(cv::Rect(1280, 720, 1280, 720)));

  if (ballVisible) {
    std::stringstream ss;
    ss << "X: " << ballPosition[0] << ", Y: " << ballPosition[1]
       << ", Z: " << ballPosition[2];
    cv::putText(screen, ss.str(), cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX,
                1, cv::Scalar(255, 255, 255));
  }

  cv::putText(screen, "FPS: " + std::to_string(fps), cv::Point(10, 30),
              cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
  writer << screen;
  cv::resize(screen, windowScreen, windowScreen.size());
  cv::imshow(WINDOW_NAME, windowScreen);
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
void Visualizer::setScreen(const cv::Mat &screen) { visionScreen = screen; }
Visualizer::~Visualizer() {
  writer.release();
  cv::destroyWindow(WINDOW_NAME);
}
