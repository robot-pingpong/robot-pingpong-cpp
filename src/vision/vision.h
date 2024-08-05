#ifndef VISION_H
#define VISION_H
#include "tracker.h"
#include "visualizer.h"

#include <opencv2/opencv.hpp>

class Vision {
  cv::Mat screen;
  Tracker t;

public:
  Vision();
  void init(Visualizer &visualizer, bool skip = false);
  bool track(cv::Vec3d &pos);
  const cv::Mat &getScreen() { return screen; }
};

#endif // VISION_H
