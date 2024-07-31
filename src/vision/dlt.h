#ifndef DLT_H
#define DLT_H
#include <opencv2/core/types.hpp>
#include <vector>

class DLT {
public:
  static void pose(const std::vector<cv::Point3d> &objectPoints,
                   const std::vector<cv::Point2d> &imagePoints, cv::Mat &K,
                   cv::Mat &R, cv::Mat &t, cv::Mat &P);
};

#endif // DLT_H
