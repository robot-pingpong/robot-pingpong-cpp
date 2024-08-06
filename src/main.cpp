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
  Predictor predictor;
  Visualizer visualizer(predictor);
  Vision vision;
  vision.init(visualizer, true);

  try {
    Timer timer;

    cv::Vec3d ballPosition;
    double y, z = 0;

    do {
      if (vision.track(ballPosition)) {
        predictor.addBallPosition(ballPosition);
        visualizer.setBallPosition(ballPosition);
      } else {
        predictor.addMissingBallPosition();
      }
      if (predictor.predictY(y)) {
        lm.setPosition(lm.map(y, Y_TABLE_SIZE - 0.18, 0.18), false);
      } else {
        lm.setPosition(lm.map(0.5, 0, 1), false);
        arm.resetByZ(z);
      }

      if (predictor.predictZ(z)) {
        arm.move(y, z * 1000);
      }

      if (predictor.hitTarget()) {
        // arm.hitByZ(z);
      }

      visualizer.setMachinePosition(
          lm.getMappedPosition(Y_TABLE_SIZE - 0.18, 0.18));
      visualizer.setScreen(vision.getScreen());
      visualizer.render(timer.getFps());
      lm.update();
    } while (!visualizer.stopped());
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
