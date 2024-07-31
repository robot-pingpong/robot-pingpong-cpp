#ifndef SERVO_H
#define SERVO_H
#include <dynamixel_sdk.h>
#include <map>
#include <string>

#define UNIT_SCALE 0.087891

static std::map<std::string, dynamixel::PortHandler *> controllers;
static dynamixel::PortHandler *getController(const std::string &portName);

namespace Servos {
template <typename Model> struct ControlTables {};

template <typename Model> class Dynamixel {
  dynamixel::PortHandler *portHandler;
  dynamixel::PacketHandler *packetHandler;

public:
  typedef typename Servos::ControlTables<Model>::protocol_t protocol_t;
  typename protocol_t::id_t id;
  int ping();
  bool reboot();
  double getAngle();

protected:
  Dynamixel(const std::string &portName,
            typename Servos::ControlTables<Model>::id_t id);
};
} // namespace Servos

#endif // SERVO_H
