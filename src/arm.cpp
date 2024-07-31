#include "arm.h"

#include "constants.h"

#include <cassert>
#include <iostream>

Arm::Arm() {
  std::cout << base.ping() << std::endl;
  std::cout << pitchShoulder.ping() << std::endl;
  std::cout << yawShoulder.ping() << std::endl;
  std::cout << elbow.ping() << std::endl;
  std::cout << wrist.ping() << std::endl;

  // controller.send(base.ping());
  // dynamixel::StatusPacket<dynamixel::Protocol2> status_packet;
  // controller.recv();
  // controller.send(base.get_moving_speed());
  // controller.recv(status_packet);
  // base.parse_moving_speed(status_packet);
  //
  // controller.spee

  // assert(base.ping() == "MX-64(2.0)");
  // assert(yawShoulder.ping() == "MX-64(2.0)");
  // assert(pitchShoulder.ping() == "MX-64(2.0)");
  // assert(elbow.ping() == "MX-28(2.0)");
  // assert(wrist.ping() == "MX-28(2.0)");
}
