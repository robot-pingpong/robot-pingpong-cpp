#include <sstream>
#include "tracker.h"
#include "constants.h"

Tracker::Tracker(cv::Mat &screen) : first(0), second(1) {
  capture(true);
  halfSize = cv::Size(firstFrame.cols / 2, firstFrame.rows / 2);
  firstFrame.copyTo(screen);
}

void Tracker::setMask() {
  first.setGlobalMask("first mask");
  second.setGlobalMask("second mask");
}

void Tracker::setTableArea() {
  auto firstPoints = first.getTableArea();
  auto secondPoints = second.getTableArea();

  std::vector<std::vector<cv::Point3f>> objectPoints = {
          {
                  {0, Y_TABLE_SIZE, 0},
                  {X_TABLE_SIZE, Y_TABLE_SIZE, 0},
                  {X_TABLE_SIZE, 0, 0},
                  {0, 0, 0},
                  {X_TABLE_SIZE / 2, -0.1525, 0.15},
                  {X_TABLE_SIZE / 2, Y_TABLE_SIZE + 0.1525, 0.15}
          }
  };

  std::vector<std::vector<cv::Point2f>> fs = {
          {
                  {firstPoints[0].x, firstPoints[0].y},
                  {firstPoints[1].x, firstPoints[1].y},
                  {firstPoints[2].x, firstPoints[2].y},
                  {firstPoints[3].x, firstPoints[3].y},
                  {firstPoints[4].x, firstPoints[4].y},
                  {firstPoints[5].x, firstPoints[5].y},
          }
  };
  std::vector<std::vector<cv::Point2f>> ss = {
          {
                  {secondPoints[2].x, secondPoints[2].y},
                  {secondPoints[3].x, secondPoints[3].y},
                  {secondPoints[0].x, secondPoints[0].y},
                  {secondPoints[1].x, secondPoints[1].y},
                  {secondPoints[5].x, secondPoints[5].y},
                  {secondPoints[4].x, secondPoints[4].y},
          }
  };
  auto fCameraMatrix = cv::initCameraMatrix2D(objectPoints, fs, firstFrame.size());
  cv::Mat fDistCoeffs, fRVect, fTVect;
  auto fRms = cv::calibrateCamera(
          objectPoints, fs, firstFrame.size(),
          fCameraMatrix, fDistCoeffs, fRVect, fTVect,
          cv::CALIB_USE_INTRINSIC_GUESS | cv::CALIB_FIX_FOCAL_LENGTH | cv::CALIB_FIX_ASPECT_RATIO |
          cv::CALIB_FIX_PRINCIPAL_POINT
  );

  auto sCameraMatrix = cv::initCameraMatrix2D(objectPoints, ss, secondFrame.size());
  cv::Mat sDistCoeffs, sRVect, sTVect;
  auto sRms = cv::calibrateCamera(
          objectPoints, ss, secondFrame.size(),
          sCameraMatrix, sDistCoeffs, sRVect, sTVect,
          cv::CALIB_USE_INTRINSIC_GUESS | cv::CALIB_FIX_FOCAL_LENGTH | cv::CALIB_FIX_ASPECT_RATIO |
          cv::CALIB_FIX_PRINCIPAL_POINT
  );

  std::cout << fRms << ", " << sRms << std::endl;
  cv::Mat R, T, E, F;
  cv::stereoCalibrate(
          objectPoints, fs, ss,
          fCameraMatrix, fDistCoeffs, sCameraMatrix, sDistCoeffs,
          firstFrame.size(), R, T, E, F
  );

  cv::Mat fR, sR, Q;
  cv::stereoRectify(
          fCameraMatrix, fDistCoeffs, sCameraMatrix, sDistCoeffs,
          firstFrame.size(), R, T, fR, sR, firstProjectionMatrix, secondProjectionMatrix, Q
  );
}

void Tracker::capture(const bool render) {
  first.captureFrame();
  second.captureFrame();
  if (render) {
    int x1, y1, x2, y2;
    first.render(firstFrame, x1, y1);
    second.render(secondFrame, x2, y2);
  }
}

void Tracker::render(const cv::Mat &screen) {
  capture();
  int x1, y1, x2, y2;
  const auto firstSuccess = first.render(firstFrame, x1, y1);
  const auto secondSuccess = second.render(secondFrame, x2, y2);

  cv::resize(firstFrame, firstFrame, halfSize);
  cv::resize(secondFrame, secondFrame, halfSize);

  firstFrame.copyTo(screen(cv::Rect(0, 0, halfSize.width, halfSize.height)));
  secondFrame.copyTo(screen(cv::Rect(halfSize.width, 0, halfSize.width, halfSize.height)));

  if (!firstSuccess || !secondSuccess) {
    return;
  }
  const std::vector firstPoint = {cv::Point2f(static_cast<float>(x1), static_cast<float>(y1))};
  const std::vector secondPoint = {cv::Point2f(static_cast<float>(x2), static_cast<float>(y2))};
  cv::Mat point3d;
  cv::triangulatePoints(
          firstProjectionMatrix, secondProjectionMatrix,
          firstPoint, secondPoint, point3d
  );

  cv::Mat point3dNormalized;
  cv::convertPointsFromHomogeneous(point3d.t(), point3dNormalized);
  point3dNormalized = point3dNormalized.t();

  std::stringstream ss;
  ss << "X: " << point3dNormalized.at<float>(0, 0) << ", Y: " << point3dNormalized.at<float>(0, 1) << ", Z: " <<
     point3dNormalized.at<float>(0, 2);
  cv::putText(screen, ss.str(), cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
}
