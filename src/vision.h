#ifndef VISION_H
#define VISION_H
#include "tracker.h"
#include "visualizer.h"

#include <opencv2/opencv.hpp>

class Vision {
  cv::Mat screen;
  Visualizer visualizer;
  Tracker t;
  cv::VideoWriter writer;
  bool lastSuccess = false;

public:
  Vision();
  void init();
  [[nodiscard]] bool stopped() const;
  void setMachinePosition(double machinePosition);
  bool track(cv::Vec3d &pos);
  void render(const double fps);
  ~Vision();
};

#endif // VISION_H
