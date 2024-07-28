#ifndef ROBOT_PINGPONG_CPP_CAPTURE_H
#define ROBOT_PINGPONG_CPP_CAPTURE_H

#include <opencv2/opencv.hpp>

class Capture {
private:
    cv::VideoCapture capture;
    cv::Ptr<cv::BackgroundSubtractorMOG2> bgSubtractor;
    cv::Mat globalMask;
public:
    Capture(int deviceId, int apiPreference = cv::CAP_ANY);

    void setGlobalMask(const std::string windowName = "Global Mask");

    void getTableArea(const std::string windowName = "Table Area");

    ~Capture();
};


#endif //ROBOT_PINGPONG_CPP_CAPTURE_H
