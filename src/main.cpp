#include "capture.h"
#include "constants.h"
#include "linear_motor.h"
#include "tracker.h"

int main() {
  auto lm = LinearMotor(0);
  lm.guessLimits();

  cv::Mat screen;
  cv::viz::Viz3d visualizer("visualizer");
  visualizer.setWindowSize(cv::Size(1280, 360));
  Tracker t(screen, visualizer);
  t.setMask();
  t.setTableArea(visualizer);

  auto prev = std::chrono::steady_clock::now();
  std::stringstream fileName;
  fileName << "output" << std::time(nullptr) << ".mkv";
  cv::VideoWriter writer(fileName.str(),
                         cv::VideoWriter::fourcc('X', '2', '6', '4'), 30,
                         cv::Size(1280, 720));
  visualizer.spinOnce();
  while (!visualizer.wasStopped()) {
    const bool success = t.render(screen, visualizer);

    auto curr = std::chrono::steady_clock::now();
    const auto elapsed = static_cast<double>(
        std::chrono::duration_cast<std::chrono::milliseconds>(curr - prev)
            .count());
    const auto fps = 1000.0 / elapsed;
    prev = curr;

    cv::putText(screen, "FPS: " + std::to_string(fps), cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
    if (success) {
      std::stringstream ss;
      ss << "X: " << t.pos[0] << ", Y: " << t.pos[1] << ", Z: " << t.pos[2];
      cv::putText(screen, ss.str(), cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX,
                  1, cv::Scalar(255, 255, 255));
      lm.setPosition(lm.map(t.pos[1], Y_TABLE_SIZE, 0), false);
    }

    visualizer.spinOnce();
    visualizer.getScreenshot().copyTo(screen(cv::Rect(0, 360, 1280, 360)));
    cv::imshow("screen", screen);
    writer.write(screen);
  }
  writer.release();
  return 0;
}
