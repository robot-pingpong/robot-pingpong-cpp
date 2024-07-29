#ifndef ROBOT_PINGPONG_CPP_CAPTURE_H
#define ROBOT_PINGPONG_CPP_CAPTURE_H

#include <opencv2/opencv.hpp>
#include <string>

class Capture {
  cv::VideoCapture capture;
  cv::Ptr<cv::BackgroundSubtractor> bgSubtractor;
  cv::Ptr<cv::SimpleBlobDetector> detector;
  cv::Mat globalMask;
  cv::Mat frame;
  cv::Mat copy;
  cv::Mat mat;
  cv::Mat hsv;
  cv::Mat grayMask;
  cv::Mat colorMask;
  const cv::Mat morphKernel =
      cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));

public:
  explicit Capture(int deviceId, int apiPreference = cv::CAP_ANY);

  void setGlobalMask(const std::string &windowName = "Global Mask");

  std::vector<cv::Point2f>
  getTableArea(const std::string &windowName = "Table Area");

  void captureFrame();

  bool render(cv::Mat &out, cv::Point2f &point);

  ~Capture() = default;
};

#endif // ROBOT_PINGPONG_CPP_CAPTURE_H
