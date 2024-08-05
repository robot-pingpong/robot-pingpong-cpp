#include "predictor.h"

#include "constants.h"

#define TARGET_X (X_TABLE_SIZE - 0.35)

void Predictor::addBallPosition(const cv::Vec3d &position) {
  // if (!history.empty()) {
  //   if (isDistanceIgnorable(position, history.back())) {
  //     reset();
  //     return;
  //   }
  // }
  history.push_back(position);
  if (history.size() < 3)
    return;

  const auto first = history[history.size() - 3];
  const auto mid = history[history.size() - 2];
  if (first[0] > mid[0] && mid[0] > position[0]) {
    reset();
    return;
  }

  if (checkIsBounded(first, mid, position)) {
    boundIndicies.push_back(history.size() - 2);

    if (position[0] > X_TABLE_SIZE / 2) {
      const auto refX = TARGET_X - (TARGET_X - position[0]) * 2;
      const auto &ref = getNearestPositionWithX(refX);
      targetZ = ref[2];
      zSet = true;
    }
  }

  if (!ySet && position[0] > X_TABLE_SIZE / 2) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (const auto &pos : history) {
      sumX += pos[0];
      sumY += pos[1];
      sumXY += pos[0] * pos[1];
      sumX2 += pos[0] * pos[0];
    }
    const auto n = static_cast<double>(history.size());
    const auto a = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    const auto b = (sumY - a * sumX) / n;
    targetY = a * TARGET_X + b;
    ySet = true;
  }

  if (position[0] > TARGET_X / 2) {
    hit = true;
  }
}

void Predictor::addMissingBallPosition() {
  if (history.empty())
    return;
  missCount += 1;
  if (missCount > 10) {
    reset();
  }
}
bool Predictor::predictY(double &y) const {
  y = targetY;
  return ySet;
}
bool Predictor::predictZ(double &z) const {
  z = targetZ;
  return ySet;
}
bool Predictor::hitTarget() {
  if (hit && !hitDone) {
    hitDone = true;
    return true;
  }
  return false;
}

void Predictor::reset() {
  history.clear();
  boundIndicies.clear();
  missCount = 0;
  ySet = false;
  zSet = false;
  hit = false;
  hitDone = false;
}

bool Predictor::isDistanceIgnorable(const cv::Vec3d &a,
                                    const cv::Vec3d &b) const {
  return cv::norm(a - b) > 0.1 * (missCount + 1);
}

bool Predictor::checkIsBounded(const cv::Vec3d &a, const cv::Vec3d &b,
                               const cv::Vec3d &c) {
  return (a[2] > b[2] && b[2] < c[2]);
}
const cv::Vec3d &Predictor::getNearestPositionWithX(const double x) const {
  double minDist = std::numeric_limits<double>::max();
  size_t minIndex = 0;
  for (size_t i = 0; i < history.size(); i++) {
    const auto &pos = history[i];
    if (const auto dist = std::abs(pos[0] - x); dist < minDist) {
      minDist = dist;
      minIndex = i;
    }
  }
  return history[minIndex];
}
