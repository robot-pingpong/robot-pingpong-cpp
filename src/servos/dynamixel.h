#ifndef SERVO_H
#define SERVO_H
#include <dynamixel_sdk.h>
#include <map>
#include <string>

static std::map<std::string, dynamixel::PortHandler *> controllers;
static dynamixel::PortHandler *getController(const std::string &portName);

namespace Servos {
template <typename Model> struct ControlTables {};

template <typename Model> class Dynamixel {
  dynamixel::PortHandler *controller;

public:
  typedef typename Servos::ControlTables<Model>::protocol_t protocol_t;
  typename protocol_t::id_t id;

protected:
  Dynamixel(const std::string &portName,
            typename Servos::ControlTables<Model>::id_t id);
};
} // namespace Servos

#endif // SERVO_H
