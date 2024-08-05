#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>

class Visualizer {
  cv::Mat screen;
  cv::Mat top;
  cv::Mat right;
  cv::Vec3d ballPosition;
  bool ballVisible;
  double machinePosition;
  std::list<std::pair<cv::viz::WCameraPosition, cv::Affine3d>> cameras;

  void renderTop();
  void renderRight();
  static cv::Point convertToTop(const cv::Vec3d &vec);
  static cv::Point convertToRight(const cv::Vec3d &vec);

public:
  Visualizer();
  void addCamera(int index, const cv::Matx33d &matrix, const cv::Mat &rotation,
                 const cv::Mat &position);
  const cv::Mat &render();
  void setBallPosition(const cv::Vec3d &vec);
  void setMachinePosition(double y);
};

#endif // VISUALIZER_H
