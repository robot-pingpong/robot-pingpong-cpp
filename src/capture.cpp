#include <iostream>

#include "capture.h"
#include "constants.h"

Capture::Capture(const int deviceId, const int apiPreference)
    : capture(deviceId, apiPreference) {
  if (!capture.isOpened()) {
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
  bgSubtractor = cv::createBackgroundSubtractorKNN();
  capture >> img;
  globalMask = cv::Mat(img.size(), CV_8UC1, cv::Scalar(255));
}

void Capture::setGlobalMask(const std::string &windowName) {
  cv::Mat screen;
  std::vector<cv::Point> points;

  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
  cv::moveWindow(windowName, 0, 0);
  cv::setMouseCallback(
      windowName,
      [](const int event, int x, int y, int flags, void *userdata) {
        if (event == cv::EVENT_LBUTTONDOWN) {
          auto *ps = static_cast<std::vector<cv::Point> *>(userdata);
          ps->emplace_back(x, y);
        }
      },
      &points);

  while (true) {
    capture >> screen;
    for (auto it = points.begin(); it != points.end(); ++it) {
      cv::circle(screen, *it, 5, cv::Scalar(0, 0, 255), -1);
      cv::line(screen, *it,
               std::next(it) == points.end() ? *points.begin() : *std::next(it),
               cv::Scalar(0, 0, 255), 2);
    }
    if (points.size() > 2) {
      cv::Mat overlay = cv::Mat::zeros(screen.size(), screen.type());

      cv::fillPoly(overlay, {points}, cv::Scalar(255));
      cv::addWeighted(screen, 1, overlay, 0.5, 0, screen);
    }
    cv::imshow(windowName, screen);
    const int key = cv::waitKey(1);
    if (key == 27)
      break;
    if (key == 8 || key == 127)
      points.pop_back();
  }
  globalMask = cv::Scalar(0);
  cv::fillPoly(globalMask, {points}, cv::Scalar(255));
  cv::destroyWindow(windowName);
}

std::vector<cv::Point2f> Capture::getTableArea(const std::string &windowName) {
  cv::Mat screen;
  std::vector<cv::Point2f> points;

  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
  cv::moveWindow(windowName, 0, 0);
  cv::setMouseCallback(
      windowName,
      [](const int event, const int x, const int y, int flags, void *userdata) {
        if (event == cv::EVENT_LBUTTONDOWN) {
          auto *ps = static_cast<std::vector<cv::Point2f> *>(userdata);
          ps->emplace_back(static_cast<float>(x), static_cast<float>(y));
        }
      },
      &points);

  while (true) {
    capture >> screen;
    for (auto it = points.begin(); it != points.end(); ++it) {
      cv::circle(screen, *it, 5, cv::Scalar(0, 0, 255), -1);
      cv::putText(screen,
                  "P" + std::to_string(std::distance(points.begin(), it) + 1),
                  *it, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
    }
    cv::imshow(windowName, screen);
    const int key = cv::waitKey(1);
    if (key == 27)
      break;
    if (key == 8 || key == 127)
      points.pop_back();
  }
  if (points.size() != 6) {
    std::cerr << "Error: Table area must have 6 points." << std::endl;
    exit(1);
  }
  cv::destroyWindow(windowName);
  return points;
}

void Capture::captureFrame() { capture >> frame; }

bool Capture::render(cv::Mat &out, cv::Point2f &point) {
  frame.copyTo(copy);
  out = cv::Scalar(0, 0, 0);
  cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
  cv::inRange(hsv, ORANGE_HSV_LOWER, ORANGE_HSV_UPPER, grayMask);
  cv::bitwise_and(grayMask, globalMask, grayMask);
  cv::bitwise_and(hsv, hsv, out, grayMask);
  out.copyTo(hsv);
  cv::cvtColor(out, out, cv::COLOR_HSV2BGR);

  out.copyTo(copy);
  out = cv::Scalar(0, 0, 0);
  cv::GaussianBlur(copy, copy, cv::Size(5, 5), 0);
  bgSubtractor->apply(copy, grayMask);
  cv::morphologyEx(grayMask, grayMask, cv::MORPH_OPEN, morphKernel,
                   cv::Point(-1, -1), 2);
  cv::bitwise_and(grayMask, globalMask, grayMask);
  cv::bitwise_and(copy, copy, out, grayMask);

  out.copyTo(copy);
  out = cv::Scalar(0, 0, 0);
  // cv::GaussianBlur(copy, copy, cv::Size(5, 5), 0);
  // cv::Canny(copy, grayMask, CANNY_THRESHOLD, CANNY_THRESHOLD);
  // cv::morphologyEx(grayMask, grayMask, cv::MORPH_CLOSE, morphKernel,
  //                  cv::Point(-1, -1), 3);
  // cv::morphologyEx(grayMask, grayMask, cv::MORPH_OPEN, morphKernel,
  //                  cv::Point(-1, -1), 2);
  // cv::morphologyEx(grayMask, grayMask, cv::MORPH_DILATE, morphKernel,
  //                  cv::Point(-1, -1), 2);

  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(grayMask, contours, cv::RETR_EXTERNAL,
                   cv::CHAIN_APPROX_SIMPLE);

  auto minColorSimilarity = MIN_COLOR_SIMILARITY;
  int maxContourIndex = -1;

  for (int i = 0; i < contours.size(); ++i) {
    const auto area = cv::contourArea(contours.at(i));
    const auto perimeter = cv::arcLength(contours.at(i), true);
    if (const auto circularity = 4 * M_PI * area / (perimeter * perimeter);
        circularity < CIRCULARITY_THRESHOLD) {
      cv::drawContours(copy, contours, i, YELLOW, 2);
      continue;
    }

    const auto rect = cv::boundingRect(contours.at(i));
    if (const auto ratio = static_cast<double>(rect.width) / rect.height;
        ratio > RATIO_THRESHOLD || ratio < 1 / RATIO_THRESHOLD) {
      cv::drawContours(copy, contours, i, CYAN, 2);
      continue;
    }

    grayMask = cv::Scalar(0);
    cv::drawContours(grayMask, contours, i, cv::Scalar(255), cv::FILLED);
    cv::drawContours(copy, contours, i, GREEN, 2);
    auto meanColor = cv::mean(hsv, grayMask);
    if (const auto diff = cv::norm(meanColor, ORANGE_HSV, cv::NORM_L2);
        diff < minColorSimilarity) {
      minColorSimilarity = diff;
      maxContourIndex = i;
    }
  }

  cv::addWeighted(frame, 0.1, copy, 0.9, 0, out);

  if (maxContourIndex != -1) {
    cv::drawContours(out, contours, maxContourIndex, RED, 2);
    grayMask = cv::Scalar(0);
    cv::drawContours(grayMask, contours, maxContourIndex, cv::Scalar(255), -1);
    cv::moments(grayMask, false);
    point.x = static_cast<float>(cv::moments(grayMask).m10 /
                                 cv::moments(grayMask).m00);
    point.y = static_cast<float>(cv::moments(grayMask).m01 /
                                 cv::moments(grayMask).m00);
    return true;
  }
  return false;
}
