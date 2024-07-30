#include "capture.h"
#include "constants.h"
#include "dynamixel.h"
#include "linear_motor.h"
#include "timer.h"
#include "vision.h"

int main() {
  auto d = Dynamixel("COM3", 1);
  std::cout << "id: 1, model name: " << d.ping() << std::endl;

  auto lm = LinearMotor(0);
  lm.guessLimits();
  Vision v;
  v.init();
  Timer t;

  cv::Vec3d ballPosition;

  do {
    if (v.track(ballPosition)) {
      lm.setPosition(lm.map(ballPosition[1], Y_TABLE_SIZE, 0), false);
    }
    v.render(t.getFps());
  } while (!v.stopped());
}
