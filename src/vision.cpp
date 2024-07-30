#include "vision.h"

#include "constants.h"

Vision::Vision() : t(screen) {
  std::stringstream fileName;
  fileName << "output" << std::time(nullptr) << ".mkv";
  writer.open(fileName.str(), cv::VideoWriter::fourcc('X', '2', '6', '4'), 30,
              cv::Size(1280, 720));
};

void Vision::init() {
  t.setMask();
  t.setTableArea(visualizer);
}
bool Vision::stopped() const { return visualizer.stopped(); }

void Vision::setMachinePosition(const double machinePosition) {
  visualizer.setMachinePosition(machinePosition);
}

bool Vision::track(cv::Vec3d &pos) {
  lastSuccess = t.render(screen);
  if (lastSuccess) {
    pos = t.pos;
  }
  return lastSuccess;
}

void Vision::render(const double fps) {
  cv::putText(screen, "FPS: " + std::to_string(fps), cv::Point(10, 30),
              cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
  if (lastSuccess) {
    std::stringstream ss;
    ss << "X: " << t.pos[0] << ", Y: " << t.pos[1] << ", Z: " << t.pos[2];
    cv::putText(screen, ss.str(), cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX,
                1, cv::Scalar(255, 255, 255));
    visualizer.setBallPosition(t.pos);
  }

  visualizer.render();
  visualizer.getScreenshot().copyTo(screen(cv::Rect(0, 360, 1280, 360)));
  cv::imshow("screen", screen);
  writer.write(screen);
}
Vision::~Vision() {
  writer.release();
  cv::destroyWindow("screen");
}
