#include "predictor.h"

#include "constants.h"
#include "utils/regression.h"

#define TARGET_X (X_TABLE_SIZE - 0.1)

void Predictor::addBallPosition(const cv::Vec3d &position) {
  // if (history.size() > 1) {
  //   if (const auto base =
  //           cv::norm(history.back() - history.at(history.size() - 2));
  //       isDistanceIgnorable(position, history.back(), base)) {
  //
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
  }

  for (const auto &pos : history) {
    // run predict if only the ball had been in the left side of the table
    if (pos[0] < X_TABLE_SIZE / 2) {
      predict(position);
      break;
    }
  }
}

void Predictor::predict(const cv::Vec3d &position) {
  if (!boundIndicies.empty()) {
    // Quadratic interpolation
    std::vector<double> srcX;
    std::vector<double> srcY;
    const size_t startIndex = boundIndicies.back();
    const size_t endIndex = history.size() - 1;
    for (size_t i = startIndex; i <= endIndex; i++) {
      srcX.push_back(history[i][0]);
      srcY.push_back(history[i][2]);
    }
    PolynomialRegression<double>::fitIt(srcX, srcY, 2, boundQuadratic);

    if (const auto targetZ = boundQuadratic.at(0) +
                             boundQuadratic.at(1) * TARGET_X +
                             boundQuadratic.at(2) * TARGET_X * TARGET_X;
        boundQuadratic.at(2) < 0 && targetZ > 0 && targetZ < 0.5) {
      std::cout << "Target Z: " << targetZ << std::endl;
      this->targetZ = targetZ;
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
  if (!yFinalSet && position[0] > X_TABLE_SIZE / 3 * 2) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    const std::vector sublist(&history[history.size() / 2],
                              &history[history.size() - 1]);
    for (const auto &pos : sublist) {
      sumX += pos[0];
      sumY += pos[1];
      sumXY += pos[0] * pos[1];
      sumX2 += pos[0] * pos[0];
    }
    const auto n = static_cast<double>(sublist.size());
    const auto a = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    const auto b = (sumY - a * sumX) / n;
    targetY = a * TARGET_X + b;
    yFinalSet = true;
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
  yFinalSet = false;
  zSet = false;
  hit = false;
  hitDone = false;
}

bool Predictor::isDistanceIgnorable(const cv::Vec3d &a, const cv::Vec3d &b,
                                    const double unit) const {
  return cv::norm(a - b) > unit * 2 * (missCount + 1);
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
