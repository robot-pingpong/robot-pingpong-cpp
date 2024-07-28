#include <iostream>

#include "capture.h"
#include "constants.h"

Capture::Capture(const int deviceId, const int apiPreference) : capture(deviceId, apiPreference)
{
    if (!capture.isOpened())
    {
        std::cerr << "Error: Could not open camera." << std::endl;
        exit(1);
    }
    capture.set(cv::CAP_PROP_FRAME_WIDTH, CAPTURE_WIDTH);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, CAPTURE_HEIGHT);
    capture.set(cv::CAP_PROP_FPS, 60);
    capture.set(cv::CAP_PROP_EXPOSURE, -7);
    capture.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
    capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

    cv::Mat img;
    bgSubtractor = cv::createBackgroundSubtractorMOG2();
    capture >> img;
    globalMask = cv::Mat::zeros(img.size(), CV_8UC1);
    hsvLower = cv::Mat(img.size(), CV_8UC3, cv::Scalar(8, 120, 140));
    hsvUpper = cv::Mat(img.size(), CV_8UC3, cv::Scalar(20, 255, 255));
}

void Capture::setGlobalMask(const std::string& windowName)
{
    cv::Mat screen;
    std::vector<cv::Point> points;

    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(windowName, 0, 0);
    cv::setMouseCallback(windowName, [](int event, int x, int y, int flags, void* userdata)
    {
        if (event == cv::EVENT_LBUTTONDOWN)
        {
            auto* ps = static_cast<std::vector<cv::Point>*>(userdata);
            ps->emplace_back(x, y);
        }
    }, &points);

    while (true)
    {
        capture >> screen;
        for (auto it = points.begin(); it != points.end(); it++)
        {
            cv::circle(screen, *it, 5, cv::Scalar(0, 0, 255), -1);
            cv::line(
                screen, *it,
                std::next(it) == points.end() ? *points.begin() : *std::next(it),
                cv::Scalar(0, 0, 255), 2
            );
        }
        if (points.size() > 2)
        {
            cv::Mat overlay = cv::Mat::zeros(screen.size(), screen.type());

            cv::fillPoly(overlay, {points}, cv::Scalar(255));
            cv::addWeighted(screen, 1, overlay, 0.5, 0, screen);
        }
        cv::imshow(windowName, screen);
        int key = cv::waitKey(1);
        if (key == 27) break;
        if (key == 8 || key == 127) points.pop_back();
    }
    globalMask = cv::Mat::zeros(screen.size(), CV_8UC1);
    cv::fillPoly(globalMask, {points}, cv::Scalar(255));
}

void Capture::getTableArea(const std::string& windowName)
{
    cv::Mat screen;
    std::vector<cv::Point> points;

    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(windowName, 0, 0);
    cv::setMouseCallback(windowName, [](const int event, const int x, const int y, int flags, void* userdata)
    {
        if (event == cv::EVENT_LBUTTONDOWN)
        {
            auto* ps = static_cast<std::vector<cv::Point>*>(userdata);
            ps->emplace_back(x, y);
        }
    }, &points);

    while (true)
    {
        capture >> screen;
        for (auto it = points.begin(); it != points.end(); ++it)
        {
            cv::circle(screen, *it, 5, cv::Scalar(0, 0, 255), -1);
            cv::putText(screen, "P" + std::to_string(std::distance(points.begin(), it) + 1), *it,
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
        }
        cv::imshow(windowName, screen);
        int key = cv::waitKey(1);
        if (key == 27) break;
        if (key == 8 || key == 127) points.pop_back();
    }
    if (points.size() != 6)
    {
        std::cerr << "Error: Table area must have 6 points." << std::endl;
        exit(1);
    }
}

void Capture::captureFrame()
{
    capture >> frame;
}

void Capture::render()
{
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, hsvLower, hsvUpper, grayMask);
    cv::bitwise_and(grayMask, globalMask, colorMask);
}
