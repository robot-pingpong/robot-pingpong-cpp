#ifndef TRACKER_H
#define TRACKER_H

#include "capture.h"
#include "visualizer.h"

#include <opencv2/viz.hpp>

class Tracker {
  Capture first;
  Capture second;
  cv::Mat firstFrame;
  cv::Mat secondFrame;
  cv::Mat cameraMatrix[2];
  cv::Mat projectionMatrix[2];
  cv::Mat rmap[2][2];

  void capture(bool render = false);

public:
  explicit Tracker(cv::Mat &screen);

  void setMask(bool skip = false);

  void setTableArea(Visualizer &visualizer, bool skip = false);

  cv::Vec3d pos;
  bool render(const cv::Mat &screen);
};

#endif // TRACKER_H
