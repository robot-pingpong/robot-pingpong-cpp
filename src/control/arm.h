#ifndef ARM_H
#define ARM_H

#include "../constants.h"
#include "../dynamixel/mx28_p2.h"
#include "../dynamixel/mx64_p2.h"

#include <list>

struct ArmDictionary {
  double maxHeight;

  double baseAngle;
  double yawShoulderAngle;
  double pitchShoulderAngle;
  double elbowAngle;
  double wristBeforeAngle;
  double wristAfterAngle;
};

class Arm {
  Servos::Mx64P2 base = Servos::Mx64P2(PORT_NAME, BASE_ID);
  Servos::Mx64P2 shoulder = Servos::Mx64P2(PORT_NAME, SHOULDER_ID);
  Servos::Mx64P2 arm = Servos::Mx64P2(PORT_NAME, ARM_ID);
  Servos::Mx28P2 elbow = Servos::Mx28P2(PORT_NAME, ELBOW_ID);
  Servos::Mx28P2 wrist = Servos::Mx28P2(PORT_NAME, WRIST_ID);
  std::list<BaseMotor *> motors = {&base, &shoulder, &arm, &elbow, &wrist};
  bool resetted = false;

public:
  Arm();

  void moveByZ(double z);
  void hitByZ(double z);
  void resetByZ(double z);
};

#endif // ARM_H
