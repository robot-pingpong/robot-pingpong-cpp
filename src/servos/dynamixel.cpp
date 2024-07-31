#include "dynamixel.h"

#include "mx28_p2.h"
#include "mx64_p2.h"

#include <stdexcept>

dynamixel::PortHandler *getController(const std::string &portName) {
  if (controllers.find(portName) != controllers.end()) {
    return controllers[portName];
  }
  const auto controller =
      dynamixel::PortHandler::getPortHandler(portName.c_str());
  if (!controller->openPort()) {
    throw std::runtime_error("Failed to open port " + portName);
  }
  controllers[portName] = controller;
  return controllers[portName];
}

namespace Servos {
template class Dynamixel<Mx28P2>;
template class Dynamixel<Mx64P2>;

template <typename Motor>
Dynamixel<Motor>::Dynamixel(const std::string &portName,
                            typename Servos::ControlTables<Motor>::id_t id)
    : portHandler(getController(portName)), id(id) {
  packetHandler = dynamixel::PacketHandler::getPacketHandler(
      ControlTables<Motor>::protocol_t::version);
}
template <typename Model> int Dynamixel<Model>::ping() {
  uint16_t modelNumber;
  uint8_t error;
  if (const int result =
          packetHandler->ping(portHandler, id, &modelNumber, &error);
      result != COMM_SUCCESS) {
    throw std::runtime_error(packetHandler->getTxRxResult(result));
  }
  if (error != 0) {
    throw std::runtime_error(packetHandler->getRxPacketError(error));
  }
  if (modelNumber != ControlTables<Model>::modelNumberValue) {
    throw std::runtime_error("Model number mismatch");
  }
  return modelNumber;
}
} // namespace Servos