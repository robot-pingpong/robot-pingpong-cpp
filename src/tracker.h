#ifndef TRACKER_H
#define TRACKER_H

#include "capture.h"

#include <opencv2/viz.hpp>

class Tracker {
  Capture first;
  Capture second;
  cv::Mat firstFrame;
  cv::Mat secondFrame;
  cv::Size halfSize;
  cv::Mat firstProjectionMatrix;
  cv::Mat secondProjectionMatrix;

  void capture(bool render = false);

public:
  explicit Tracker(cv::Mat &screen, cv::viz::Viz3d &visualizer);

  void setMask();

  void setTableArea();

  void render(const cv::Mat &screen, const cv::viz::Viz3d &visualizer);
};

#endif // TRACKER_H
