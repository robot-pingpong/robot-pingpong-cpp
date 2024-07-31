#ifndef ARM_H
#define ARM_H

#include "constants.h"
#include "dynamixel.h"
#include "servos/mx28_p2.h"
#include "servos/mx64_p2.h"

class Arm {
  Dynamixel<Servos::Mx64P2> base =
      Dynamixel<Servos::Mx64P2>(PORT_NAME, BASE_ID);
  Dynamixel<Servos::Mx64P2> yawShoulder =
      Dynamixel<Servos::Mx64P2>(PORT_NAME, YAW_SHOULDER_ID);
  Dynamixel<Servos::Mx64P2> pitchShoulder =
      Dynamixel<Servos::Mx64P2>(PORT_NAME, PITCH_SHOULDER_ID);
  Dynamixel<Servos::Mx28P2> elbow =
      Dynamixel<Servos::Mx28P2>(PORT_NAME, ELBOW_ID);
  Dynamixel<Servos::Mx28P2> wrist =
      Dynamixel<Servos::Mx28P2>(PORT_NAME, WRIST_ID);

public:
  Arm();
};

#endif // ARM_H
