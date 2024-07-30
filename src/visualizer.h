#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>

class Visualizer {
  cv::viz::Viz3d visualizer = cv::viz::Viz3d("visualizer");
  cv::viz::WSphere ball = cv::viz::WSphere(cv::Point3f(0, 0, 0), 0.1);

public:
  Visualizer();
  void addCamera(int index, const cv::Matx33d &matrix, const cv::Mat &rotation,
                 const cv::Mat &position);
  void render(bool force = false);
  [[nodiscard]] bool stopped() const;
  void setBallPosition(const cv::Vec3d &vec);
  cv::Mat getScreenshot() const;
};

#endif // VISUALIZER_H
