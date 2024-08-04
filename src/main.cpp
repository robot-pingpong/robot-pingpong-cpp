#include "constants.h"
#include "control/arm.h"
#include "control/linear_motor.h"
#include "predictor.h"
#include "utils/timer.h"
#include "vision/vision.h"

int main() {
  Arm arm;
  auto lm = LinearMotor(0);
  lm.guessLimits();
  Vision vision;
  vision.init(true);
  Timer timer;
  Predictor predictor;

  cv::Vec3d ballPosition;
  double y, z = 0;

  do {
    if (vision.track(ballPosition)) {
      predictor.addBallPosition(ballPosition);
    } else {
      predictor.addMissingBallPosition();
    }
    if (predictor.predictY(y)) {
      lm.setPosition(lm.map(y - 0.45, Y_TABLE_SIZE + 0.1, -0.1), false);
    } else {
      lm.setPosition(lm.map(0.5, 0, 1), false);
      // arm.resetByZ(z);
    }

    if (predictor.predictZ(z)) {
      // arm.moveByZ(z);
    }

    if (predictor.hitTarget()) {
      // arm.hitByZ(z);
    }

    vision.setMachinePosition(lm.getMappedPosition(Y_TABLE_SIZE, 0));
    vision.render(timer.getFps());
    lm.update();
  } while (!vision.stopped());
}
