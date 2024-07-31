#include "arm.h"
#include "constants.h"
#include "linear_motor.h"
#include "timer.h"
#include "vision.h"

int main() {
  Arm arm;

  auto lm = LinearMotor(0);
  lm.guessLimits();
  Vision vision;
  vision.init();
  Timer timer;

  cv::Vec3d ballPosition;

  do {
    if (vision.track(ballPosition)) {
      lm.setPosition(lm.map(ballPosition[1], Y_TABLE_SIZE, 0), false);
    }
    vision.render(timer.getFps());
  } while (!vision.stopped());
}
