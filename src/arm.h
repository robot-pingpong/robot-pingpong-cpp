#ifndef ARM_H
#define ARM_H

#include "constants.h"
#include "servos/mx28_p2.h"
#include "servos/mx64_p2.h"

class Arm {
  Servos::Mx64P2 base = Servos::Mx64P2(PORT_NAME, BASE_ID);
  Servos::Mx64P2 yawShoulder = Servos::Mx64P2(PORT_NAME, YAW_SHOULDER_ID);
  Servos::Mx64P2 pitchShoulder = Servos::Mx64P2(PORT_NAME, PITCH_SHOULDER_ID);
  Servos::Mx28P2 elbow = Servos::Mx28P2(PORT_NAME, ELBOW_ID);
  Servos::Mx28P2 wrist = Servos::Mx28P2(PORT_NAME, WRIST_ID);

public:
  Arm();
};

#endif // ARM_H
