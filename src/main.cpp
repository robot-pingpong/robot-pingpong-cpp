#include "capture.h"
#include "tracker.h"

int main() {
  cv::Mat screen;
  cv::viz::Viz3d visualizer("visualizer");
  visualizer.spinOnce();
  Tracker t(screen, visualizer);
  t.setMask();
  t.setTableArea();

  auto prev = std::chrono::steady_clock::now();
  std::stringstream fileName;
  fileName << "output" << std::time(nullptr) << ".avi";
  cv::VideoWriter writer(fileName.str(),
                         cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30,
                         cv::Size(1280, 720));
  while (!visualizer.wasStopped()) {
    t.render(screen, visualizer);

    auto curr = std::chrono::steady_clock::now();
    const auto elapsed = static_cast<double>(
        std::chrono::duration_cast<std::chrono::milliseconds>(curr - prev)
            .count());
    const auto fps = 1000.0 / elapsed;
    prev = curr;

    cv::putText(screen, "FPS: " + std::to_string(fps), cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));

    // cv::imshow("screen", screen);
    writer.write(screen);
    visualizer.spinOnce(1, true);
    // if (cv::waitKey(1) == 27)
    //   break;
  }
  writer.release();
  return 0;
}
