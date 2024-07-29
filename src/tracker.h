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
  cv::Mat cameraMatrix[2];
  cv::Mat projectionMatrix[2];
  cv::Mat rmap[2][2];

  void capture(bool render = false);

  cv::viz::WSphere ball = cv::viz::WSphere(cv::Point3f(0, 0, 0), 0.1);

public:
  explicit Tracker(cv::Mat &screen, cv::viz::Viz3d &visualizer);

  void setMask();

  void setTableArea(cv::viz::Viz3d &visualizer);

  void render(const cv::Mat &screen, cv::viz::Viz3d &visualizer);
};

#endif // TRACKER_H
