#ifndef ROBOT_PINGPONG_CPP_CAPTURE_H
#define ROBOT_PINGPONG_CPP_CAPTURE_H

#include <string>
#include <opencv2/opencv.hpp>

class Capture
{
    cv::VideoCapture capture;
    cv::Ptr<cv::BackgroundSubtractorMOG2> bgSubtractor;
    cv::Mat globalMask;
    cv::Mat frame;
    cv::Mat copy;
    cv::Mat mat;
    cv::Mat hsv;
    cv::Mat grayMask;
    cv::Mat colorMask;
    const cv::Mat morphKernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    const cv::Scalar orange = cv::Scalar(8, 150, 155);

public:
    explicit Capture(int deviceId, int apiPreference = cv::CAP_ANY);

    void setGlobalMask(const std::string& windowName = "Global Mask");

    std::vector<cv::Point2f> getTableArea(const std::string& windowName = "Table Area");

    void captureFrame();

    bool render(cv::Mat& out, int& x, int& y);

    ~Capture() = default;
};


#endif //ROBOT_PINGPONG_CPP_CAPTURE_H
