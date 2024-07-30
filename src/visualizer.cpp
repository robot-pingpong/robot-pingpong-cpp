#include "visualizer.h"
#include "constants.h"

Visualizer::Visualizer() {
  visualizer.setWindowSize(cv::Size(1280, 360));
  visualizer.showWidget("Coordinate Widget", cv::viz::WCoordinateSystem());
  auto table = cv::viz::WPlane(cv::Size2d(X_TABLE_SIZE, Y_TABLE_SIZE));

  std::vector<cv::Point3d> objectPoints = OBJECT_POINTS;

  for (int i = 0; i < 4; ++i) {
    auto tableLine = cv::viz::WLine(cv::Point3d(objectPoints[i]),
                                    cv::Point3d(objectPoints[(i + 1) % 4]));
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
  visualizer.showWidget(
      "machine", machine,
      cv::Affine3d(cv::Vec3d(), cv::Vec3d(X_TABLE_SIZE, Y_TABLE_SIZE / 2, 0)));
}

void Visualizer::addCamera(const int index, const cv::Matx33d &matrix,
                           const cv::Mat &rotation, const cv::Mat &position) {
  const auto camera = cv::viz::WCameraPosition(
      cv::Matx33d(matrix), 1,
      index == 0 ? cv::viz::Color::blue() : cv::viz::Color::red());
  visualizer.showWidget("camera" + std::to_string(index), camera,
                        cv::Affine3d(rotation, position));
}

void Visualizer::render(const bool force) { visualizer.spinOnce(1, force); }
bool Visualizer::stopped() const { return visualizer.wasStopped(); }
void Visualizer::setBallPosition(const cv::Vec3d &vec) {
  visualizer.setWidgetPose("ball", cv::Affine3d(cv::Vec3d(), vec));
}
cv::Mat Visualizer::getScreenshot() const { return visualizer.getScreenshot(); }

void Visualizer::setMachinePosition(const double y) {
  visualizer.setWidgetPose(
      "machine", cv::Affine3d(cv::Vec3d(), cv::Vec3d(X_TABLE_SIZE, y, 0)));
}
