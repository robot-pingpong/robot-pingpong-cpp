#include "predictor.h"

#include "constants.h"
#include "utils/regression.h"

#include <ranges>

#define TARGET_X (X_TABLE_SIZE - 0.03)
#define FINAL_PREDICTION_Y (X_TABLE_SIZE - 0.6)
#define FINAL_PREDICTION_Z (X_TABLE_SIZE - 0.6)
#define HIT_X (TARGET_X - 0.4)

Predictor::Predictor() { reset(); }

void Predictor::addBallPosition(const cv::Vec3d &position) {
  // if (history.size() > 1) {
  //   if (const auto base =
  //           cv::norm(history.back() - history.at(history.size() - 2));
  //       isDistanceIgnorable(position, history.back(), base)) {
  //
  //     return;
  //   }
  // }
  if (history.empty()) {
    constexpr auto drag = 0.8;
    // dynam: x, y, z, vx, vy, vz, az
    kalmanFilter.init(7, 3, 0, CV_64F);
    // clang-format off
    kalmanFilter.statePre = kalmanFilter.statePost = (
      cv::Mat_<double>(7, 1) <<
      position[0], position[1], position[2],
      0, 0, 0, 0
    );
    kalmanFilter.transitionMatrix = (
      cv::Mat_<double>(7, 7) <<
      1, 0, 0, 1, 0, 0, 0,
      0, 1, 0, 0, 1, 0, 0,
      0, 0, 1, 0, 0, 1, 0,

      0, 0, 0, drag, 0, 0, 0,
      0, 0, 0, 0, drag, 0, 0,
      0, 0, 0, 0, 0, drag, 1,

      0, 0, 0, 0, 0, 0, 1
    );
    kalmanFilter.processNoiseCov = (
        cv::Mat_<double>(7, 7) <<
        0.3, 0, 0, 0, 0, 0, 0,
        0, 0.3, 0, 0, 0, 0, 0,
        0, 0, 0.3, 0, 0, 0, 0,
        0, 0, 0, 0.3, 0, 0, 0,
        0, 0, 0, 0, 0.3, 0, 0,
        0, 0, 0, 0, 0, 0.3, 0,
        0, 0, 0, 0, 0, 0, 0.3
    );
    kalmanFilter.measurementMatrix = (
        cv::Mat_<double>(3, 7) <<
        1, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0
    );
    kalmanFilter.measurementNoiseCov = cv::Mat::eye(3, 3, CV_64F) * 0.1;
    // clang-format on
  }
  missCount = 0;
  history.emplace_back(std::chrono::system_clock::now(), position);
  predicted.emplace_back(std::chrono::system_clock::now(),
                         cv::Vec3d(kalmanFilter.predict().rowRange(0, 3)));
  kalmanFilter.correct(cv::Mat(position));
  if (history.size() < 3)
    return;

  const auto first = history[history.size() - 3].second;
  const auto mid = history[history.size() - 2].second;
  if (first[0] > mid[0] && mid[0] > position[0]) {
    if (position[0] < X_TABLE_SIZE / 2)
      reset();
    return;
  }

  if (checkIsBounded(first, mid, position)) {
    boundIndicies.push_back(history.size() - 2);
  }

  for (const auto &pos : predicted | std::ranges::views::values) {
    // run predict if only the ball had been in the left side of the table
    if (pos[0] < X_TABLE_SIZE / 2) {
      predict(position);
      break;
    }
  }
}

void Predictor::predict(const cv::Vec3d &position) {
  if (!zSet || position[0] < FINAL_PREDICTION_Z) {
    boundQuadratic.clear();
    // Quadratic interpolation
    std::vector<double> srcX;
    std::vector<double> srcY;
    for (const auto &pos :
         std::vector(&history[boundIndicies.empty() ? 0 : boundIndicies.back()],
                     &history.back()) |
             std::ranges::views::values) {
      srcX.push_back(pos[0]);
      srcY.push_back(pos[2]);
    }
    if (srcX.size() > 2) {
      boundQuadratic.emplace_back();
      PolynomialRegression<double>::fitIt(srcX, srcY, 2, boundQuadratic.back());

      if (boundQuadratic.back().at(2) < 0) {
        for (int count = 0; count < 10; ++count) {
          if (const auto targetZ =
                  boundQuadratic.back().at(0) +
                  boundQuadratic.back().at(1) * TARGET_X +
                  boundQuadratic.back().at(2) * TARGET_X * TARGET_X;
              targetZ > 0) {
            if (targetZ < 0.5) {
              this->targetZ = targetZ;
              zSet = true;
            }
            break;
          }

          const auto a = boundQuadratic.back().at(2);
          const auto b = boundQuadratic.back().at(1);
          const auto c = boundQuadratic.back().at(0);
          const auto boundX = (-b - std::sqrt(b * b - 4 * a * c)) / (2 * a);
          if (isnan(boundX)) {
            break;
          }
          const auto p = -b / (2 * a);
          const auto q = c - a * p * p;
          const auto newA = a * 0.9;
          const auto newP = p + (boundX - p) * 2;
          const auto newQ = q * 0.9;
          if (0 < boundX && boundX < TARGET_X * 3) {
            boundQuadratic.emplace_back(std::vector{
                newA * newP * newP + newQ,
                -2 * newA * newP,
                newA,
            });
          } else {
            break;
          }
        }
      }
    }
  }

  if (!ySet && position[0] > X_TABLE_SIZE / 2) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (const auto &pos : predicted | std::ranges::views::values) {
      sumX += pos[0];
      sumY += pos[1];
      sumXY += pos[0] * pos[1];
      sumX2 += pos[0] * pos[0];
    }
    const auto n = static_cast<double>(predicted.size());
    const auto a = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    const auto b = (sumY - a * sumX) / n;
    targetY = a * TARGET_X + b;
    ySet = true;
  }
  if (!yFinalSet && position[0] > FINAL_PREDICTION_Y) {
    targetY = position[1];
    yFinalSet = true;
  }

  if (position[0] > HIT_X) {
    hit = true;
  }
}

void Predictor::addMissingBallPosition() {
  if (history.empty())
    return;
  missCount += 1;
  if (missCount > 30) {
    reset();
  }
}

bool Predictor::predictY(double &y) const {
  if (isnan(y))
    return false;
  y = targetY;
  return ySet;
}

bool Predictor::predictZ(double &z) const {
  if (isnan(z))
    return false;
  z = targetZ;
  return zSet;
}

bool Predictor::hitTarget() const { return hit; }

cv::Vec3d Predictor::getVelocity() const {
  if (history.size() < 2)
    return {};
  const auto &[t1, pos1] = history[history.size() - 2];
  const auto &[t2, pos2] = history[history.size() - 1];
  const auto dt =
      static_cast<double>(
          std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)
              .count()) /
      1000.0;
  return (pos2 - pos1) / dt;
}

cv::Vec3d Predictor::getAcceleration() const {
  if (history.size() < 3)
    return {};
  const auto &[t1, pos1] = history[history.size() - 3];
  const auto &[t2, pos2] = history[history.size() - 2];
  const auto &[t3, pos3] = history[history.size() - 1];
  const auto dt1 =
      static_cast<double>(
          std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)
              .count()) /
      1000.0;
  const auto dt2 =
      static_cast<double>(
          std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2)
              .count()) /
      1000.0;
  const auto v1 = (pos2 - pos1) / dt1;
  const auto v2 = (pos3 - pos2) / dt2;
  return (v2 - v1) / dt2;
}

void Predictor::reset() {
  history.clear();
  boundIndicies.clear();
  predicted.clear();
  missCount = 0;
  ySet = false;
  yFinalSet = false;
  zSet = false;
  hit = false;
}

bool Predictor::isDistanceIgnorable(const cv::Vec3d &a, const cv::Vec3d &b,
                                    const double unit) const {
  return cv::norm(a - b) > unit * 8 * (missCount + 1);
}

bool Predictor::checkIsBounded(const cv::Vec3d &a, const cv::Vec3d &b,
                               const cv::Vec3d &c) {
  return (a[2] > b[2] && b[2] < c[2]);
}

const cv::Vec3d &Predictor::getNearestPositionWithX(const double x) const {
  double minDist = std::numeric_limits<double>::max();
  size_t minIndex = 0;
  for (size_t i = 0; i < history.size(); i++) {
    const auto &[_, pos] = history[i];
    if (const auto dist = std::abs(pos[0] - x); dist < minDist) {
      minDist = dist;
      minIndex = i;
    }
  }
  return history[minIndex].second;
}
