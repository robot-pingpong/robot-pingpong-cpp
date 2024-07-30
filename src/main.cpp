#include "capture.h"
#include "constants.h"
#include "dynamixel.h"
#include "linear_motor.h"
#include "timer.h"
#include "tracker.h"
#include "visualizer.h"

int main() {
  auto d = Dynamixel("COM3", 1);
  std::cout << "id: 1, model name: " << d.ping() << std::endl;
  // auto lm = LinearMotor(0);
  // lm.guessLimits();
  //
  // cv::Mat screen;
  // Visualizer visualizer;
  // Tracker t(screen);
  // t.setMask();
  // t.setTableArea(visualizer);
  //
  // Timer timer;
  // std::stringstream fileName;
  // fileName << "output" << std::time(nullptr) << ".mkv";
  // cv::VideoWriter writer(fileName.str(),
  //                        cv::VideoWriter::fourcc('X', '2', '6', '4'), 30,
  //                        cv::Size(1280, 720));
  // visualizer.render();
  // while (!visualizer.stopped()) {
  //   const bool success = t.render(screen);
  //
  //   cv::putText(screen, "FPS: " + std::to_string(timer.getFps()),
  //               cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1,
  //               cv::Scalar(0, 0, 255));
  //   if (success) {
  //     std::stringstream ss;
  //     ss << "X: " << t.pos[0] << ", Y: " << t.pos[1] << ", Z: " << t.pos[2];
  //     cv::putText(screen, ss.str(), cv::Point(10, 80),
  //     cv::FONT_HERSHEY_SIMPLEX,
  //                 1, cv::Scalar(255, 255, 255));
  //     lm.setPosition(lm.map(t.pos[1], Y_TABLE_SIZE, 0), false);
  //     visualizer.setBallPosition(t.pos);
  //   }
  //   visualizer.setMachinePosition(lm.getMappedPosition(Y_TABLE_SIZE, 0));
  //
  //   visualizer.render();
  //   visualizer.getScreenshot().copyTo(screen(cv::Rect(0, 360, 1280, 360)));
  //   cv::imshow("screen", screen);
  //   writer.write(screen);
  // }
  // writer.release();
  // return 0;
}
