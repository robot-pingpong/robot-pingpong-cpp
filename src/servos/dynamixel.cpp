#include "dynamixel.h"

#include "mx28_p2.h"
#include "mx64_p2.h"

#include <stdexcept>

#define READ(N)                                                                \
  {                                                                            \
    uint8_t error;                                                             \
    if (const int result = packetHandler->read##N##ByteTxRx(                   \
            portHandler, id, address, &value, &error);                         \
        result != COMM_SUCCESS) {                                              \
      throw std::runtime_error(packetHandler->getTxRxResult(result));          \
    }                                                                          \
    if (error != 0) {                                                          \
      throw std::runtime_error(packetHandler->getRxPacketError(error));        \
    }                                                                          \
  }

#define SREAD(N, M)                                                            \
  {                                                                            \
    uint##M##_t v;                                                             \
    uint8_t error;                                                             \
    if (const int result = packetHandler->read##N##ByteTxRx(                   \
            portHandler, id, address, &v, &error);                             \
        result != COMM_SUCCESS) {                                              \
      throw std::runtime_error(packetHandler->getTxRxResult(result));          \
    }                                                                          \
    if (error != 0) {                                                          \
      throw std::runtime_error(packetHandler->getRxPacketError(error));        \
    }                                                                          \
    value = static_cast<int##M##_t>(v);                                        \
  }

#define WRITE(N)                                                               \
  {                                                                            \
    uint8_t error;                                                             \
    if (const int result = packetHandler->write##N##ByteTxRx(                  \
            portHandler, id, address, value, &error);                          \
        result != COMM_SUCCESS) {                                              \
      throw std::runtime_error(packetHandler->getTxRxResult(result));          \
    }                                                                          \
    if (error != 0) {                                                          \
      throw std::runtime_error(packetHandler->getRxPacketError(error));        \
    }                                                                          \
  }

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

template <typename Model>
void Dynamixel<Model>::readByte(uint16_t address, int8_t &value) SREAD(1, 8);
template <typename Model>
void Dynamixel<Model>::readByte(uint16_t address, int16_t &value) SREAD(2, 16);
template <typename Model>
void Dynamixel<Model>::readByte(uint16_t address, int32_t &value) SREAD(4, 32);
template <typename Model>
void Dynamixel<Model>::readByte(uint16_t address, uint8_t &value) READ(1);
template <typename Model>
void Dynamixel<Model>::readByte(uint16_t address, uint16_t &value) READ(2);
template <typename Model>
void Dynamixel<Model>::readByte(uint16_t address, uint32_t &value) READ(4);
template <typename Model>
void Dynamixel<Model>::writeByte(uint16_t address, int8_t value) WRITE(1);
template <typename Model>
void Dynamixel<Model>::writeByte(uint16_t address, int16_t value) WRITE(2);
template <typename Model>
void Dynamixel<Model>::writeByte(uint16_t address, int32_t value) WRITE(4);
template <typename Model>
void Dynamixel<Model>::writeByte(uint16_t address, uint8_t value) WRITE(1);
template <typename Model>
void Dynamixel<Model>::writeByte(uint16_t address, uint16_t value) WRITE(2);
template <typename Model>
void Dynamixel<Model>::writeByte(uint16_t address, uint32_t value) WRITE(4);

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
template <typename Model> bool Dynamixel<Model>::reboot() {
  uint8_t error;
  if (const int result = packetHandler->reboot(portHandler, id, &error);
      result != COMM_SUCCESS) {
    throw std::runtime_error(packetHandler->getTxRxResult(result));
  }
  if (error != 0) {
    throw std::runtime_error(packetHandler->getRxPacketError(error));
  }
  return true;
}
template <typename Model> double Dynamixel<Model>::getAngle() {
  int32_t value;
  readByte(ControlTables<Model>::PresentPosition, value);
  return value * UNIT_SCALE;
}
} // namespace Servos