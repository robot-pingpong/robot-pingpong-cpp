#include "dynamixel.h"

#include "servos/mx28_p2.h"
#include "servos/mx64_p2.h"

dynamixel::PortHandler *getController(const std::string &portName) {
  if (controllers.find(portName) != controllers.end()) {
    return controllers[portName];
  }
  controllers[portName] =
      dynamixel::PortHandler::getPortHandler(portName.c_str());
  return controllers[portName];
}

template class Dynamixel<Servos::Mx28P2>;
template class Dynamixel<Servos::Mx64P2>;

template <typename Motor>
Dynamixel<Motor>::Dynamixel(const std::string &portName,
                            typename Servos::ControlTables<Motor>::id_t id)
    : controller(getController(portName)), motor(id) {}

template <typename Motor> std::string Dynamixel<Motor>::getName() const {
  return motor.getName();
}
