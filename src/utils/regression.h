#ifndef REGRESSION_H
#define REGRESSION_H
#include <vector>

void quadraticRegression(const std::vector<double> &srcX,
                         const std::vector<double> &srcY, double &b0,
                         double &b1, double &b2);

#endif // REGRESSION_H
