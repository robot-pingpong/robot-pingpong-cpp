#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <opencv2/opencv.hpp>

class Predictor {
  std::vector<cv::Vec3d> history;
  int missCount = 0;
  std::vector<size_t> boundIndicies;
  bool ySet = false;
  double targetY = 0;
  bool zSet = false;
  double targetZ = 0;
  bool hit = false;
  bool hitDone = false;
  void reset();

  [[nodiscard]] bool isDistanceIgnorable(const cv::Vec3d &a,
                                         const cv::Vec3d &b) const;
  static bool checkIsBounded(const cv::Vec3d &a, const cv::Vec3d &b,
                             const cv::Vec3d &c);
  [[nodiscard]] const cv::Vec3d &getNearestPositionWithX(double x) const;

public:
  void addBallPosition(const cv::Vec3d &position);
  void addMissingBallPosition();
  bool predictY(double &y) const;
  bool predictZ(double &z) const;
  bool hitTarget();
  [[nodiscard]] const std::vector<cv::Vec3d> &getHistory() const;
  [[nodiscard]] std::vector<cv::Vec3d> getBoundPositions() const;
};

#endif // PREDICTOR_H
