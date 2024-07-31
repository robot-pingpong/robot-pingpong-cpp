#include "arm.h"
#include "constants.h"
#include "linear_motor.h"
#include "timer.h"
#include "vision/vision.h"

int main() {
  Arm a;
  auto lm = LinearMotor(0);
  lm.guessLimits();
  Vision vision;
  vision.init();
  Timer timer;

  cv::Vec3d ballPosition;
  std::vector<cv::Vec3d> ballHistory;
  int missCount = 0;
  bool success = false;

  do {
    if (vision.track(ballPosition)) {
      ballHistory.push_back(ballPosition);
      if (ballPosition[0] > X_TABLE_SIZE / 2 && !success) {
        auto first = ballHistory[static_cast<int>(ballHistory.size()) / 3];
        auto &last = ballPosition;
        const auto dx = last[0] - first[0];
        const auto dy = last[1] - first[1];

        constexpr auto targetX = X_TABLE_SIZE - 0.35;
        const auto targetY = last[1] + dy / dx * (targetX - last[0]);

        lm.setPosition(lm.map(targetY, Y_TABLE_SIZE, 0), false);
        success = true;
      }
    } else if (!ballHistory.empty()) {
      missCount++;
      if (missCount > 10) {
        ballHistory.clear();
        success = false;
        missCount = 0;
        lm.setPosition(lm.map(0.5, 0, 1), false);
      }
    }
    vision.setMachinePosition(lm.getMappedPosition(Y_TABLE_SIZE, 0));
    vision.render(timer.getFps());
    lm.update();
  } while (!vision.stopped());
}
