#ifndef TRACKER_H
#define TRACKER_H

#include "capture.h"

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
  explicit Tracker(cv::Mat &screen);

  void setMask();

  void setTableArea();

  void render(const cv::Mat &screen);
};

#endif // TRACKER_H
