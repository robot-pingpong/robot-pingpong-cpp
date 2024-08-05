#include "regression.h"
#include <cassert>
#include <numeric>

static double square(const double sum, const double x) { return sum + x * x; }
static double cubic(const double sum, const double x) {
  return sum + x * x * x;
}
static double forth_power(const double sum, const double x) {
  return sum + x * x * x * x;
}

void quadraticRegression(const std::vector<double> &srcX,
                         const std::vector<double> &srcY, double &b0,
                         double &b1, double &b2) {
  assert(srcX.size() == srcY.size());
  const double Y = std::accumulate(srcY.begin(), srcY.end(), 0.0);
  const double X = std::accumulate(srcX.begin(), srcX.end(), 0.0);
  const double X2 = std::accumulate(srcX.begin(), srcX.end(), 0.0, square);
  const double X3 = std::accumulate(srcX.begin(), srcX.end(), 0.0, cubic);
  const double X4 = std::accumulate(srcX.begin(), srcX.end(), 0.0, forth_power);

  double K = 0.0;
  double L = 0.0;
  const int n = static_cast<int>(srcX.size());
  for (size_t i = 0; i < srcX.size(); ++i) {
    K += cubic(0, srcX.at(i));
    L += square(0, srcY.at(i));
  }

  const double denominator =
      -n * X4 * X2 + X4 * X * X + X2 * X2 * X2 + X3 * X3 * n - 2 * X3 * X * X2;
  const double b0p = -(Y * X4 * X2 - Y * X3 * X3 - X * L * X4 + X * X3 * K -
                       X2 * X2 * K + X2 * X3 * L);
  const double b1p = X * Y * X4 - X * K * X2 - L * n * X4 + X3 * n * K -
                     Y * X2 * X3 + X2 * X2 * L;
  const double b2p = -(K * n * X2 - K * X * X - X2 * X2 * Y - X3 * n * L +
                       X3 * X * Y + X * X2 * L);

  b0 = b0p / denominator;
  b1 = b1p / denominator;
  b2 = b2p / denominator;
}
