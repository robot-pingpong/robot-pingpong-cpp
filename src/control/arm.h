#ifndef ARM_H
#define ARM_H

#include "../constants.h"
#include "../dynamixel/mx28_p2.h"
#include "../dynamixel/mx64_p2.h"

#include <cmath>
#include <list>
#include <mutex>

class Arm {
  Servos::Mx64P2 base = Servos::Mx64P2(PORT_NAME, BASE_ID);
  Servos::Mx64P2 shoulder = Servos::Mx64P2(PORT_NAME, SHOULDER_ID);
  Servos::Mx64P2 elbow = Servos::Mx64P2(PORT_NAME, ELBOW_ID);
  Servos::Mx28P2 yawWrist = Servos::Mx28P2(PORT_NAME, YAW_WRIST_ID);
  Servos::Mx28P2 pitchWrist = Servos::Mx28P2(PORT_NAME, PITCH_WRIST_ID);
  std::list<BaseMotor *> motors = {&base, &shoulder, &elbow, &yawWrist,
                                   &pitchWrist};
  bool resetted = false;
  std::mutex mtx;
  static bool inverseKinematics(double x, double y, double z, double &yawTheta1,
                                double &yawTheta2, double &theta1,
                                double &theta2, double &theta3,
                                double pi = M_PI / 2, double yawPi = 0);

public:
  void init();
  void move(double y, double z, bool hitTarget);
  void resetByZ(double z);
};

#endif // ARM_H
