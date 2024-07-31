#include "dynamixel.h"

#include "mx28_p2.h"
#include "mx64_p2.h"

dynamixel::PortHandler *getController(const std::string &portName) {
  if (controllers.find(portName) != controllers.end()) {
    return controllers[portName];
  }
  controllers[portName] =
      dynamixel::PortHandler::getPortHandler(portName.c_str());
  return controllers[portName];
}

namespace Servos {
template class Dynamixel<Mx28P2>;
template class Dynamixel<Mx64P2>;

template <typename Motor>
Dynamixel<Motor>::Dynamixel(const std::string &portName,
                            typename Servos::ControlTables<Motor>::id_t id)
    : controller(getController(portName)), id(id) {}
} // namespace Servos