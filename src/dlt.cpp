#include "dlt.h"

#include <opencv2/opencv.hpp>

void DLT::pose(const std::vector<cv::Point3d> &objectPoints,
               const std::vector<cv::Point2d> &imagePoints, cv::Mat &K,
               cv::Mat &R, cv::Mat &t, cv::Mat &P) {
  const int numberOfPoints = static_cast<int>(objectPoints.size());
  cv::Mat objectMat(objectPoints);
  objectMat = objectMat.reshape(1);
  cv::hconcat(objectMat, cv::Mat::ones(numberOfPoints, 1, CV_64F), objectMat);

  cv::Mat mean, std;
  cv::Mat imageMat(imagePoints);
  cv::meanStdDev(imageMat, mean, std);
  imageMat = imageMat.reshape(1);
  cv::hconcat(imageMat, cv::Mat::ones(numberOfPoints, 1, CV_64F), imageMat);

  double initMatrix[3][3] = {
      {1.0 / std.at<double>(0), 0.0, -mean.at<double>(0) / std.at<double>(0)},
      {0.0, 1.0 / std.at<double>(1), -mean.at<double>(1) / std.at<double>(1)},
      {0.0, 0.0, 1.0},
  };
  auto normalizedMatrix = cv::Mat(3, 3, CV_64F, initMatrix);
  cv::Mat normalizedImagePoints = normalizedMatrix * imageMat.t();

  cv::Mat A = cv::Mat::zeros(3 * numberOfPoints, 12 + numberOfPoints, CV_64F);
  for (int i = 0; i < numberOfPoints; ++i) {
    objectMat(cv::Rect(0, i, 4, 1)).copyTo(A(cv::Rect(0, 3 * i + 0, 4, 1)));
    objectMat(cv::Rect(0, i, 4, 1)).copyTo(A(cv::Rect(4, 3 * i + 1, 4, 1)));
    objectMat(cv::Rect(0, i, 4, 1)).copyTo(A(cv::Rect(8, 3 * i + 2, 4, 1)));
    A(cv::Rect(12 + i, 3 * i, 1, 3)) =
        -normalizedImagePoints(cv::Rect(i, 0, 1, 3));
  }

  cv::Mat w, u, vt;
  cv::SVD::compute(A, w, u, vt);

  cv::Mat normalizedP = vt.row(vt.rows - 1).colRange(0, 12).t();
  normalizedP = normalizedP.reshape(1, 3);
  if (const cv::Mat testSign = normalizedP * objectMat.t().col(1);
      testSign.at<double>(2) < 0) {
    normalizedP = -normalizedP;
  }
  P = normalizedMatrix.inv() * normalizedP;

  cv::decomposeProjectionMatrix(P, K, R, t);
  t = -(P.colRange(0, 3).inv() * P.col(3));
  R = R.inv();
}
