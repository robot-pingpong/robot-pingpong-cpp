#include "dynamixel.h"

#include <iostream>

Dynamixel::Dynamixel(const std::string &portName, const int id)
    : portHandler(dynamixel::PortHandler::getPortHandler(portName.c_str())),
      packetHandler(dynamixel::PacketHandler::getPacketHandler()), id(id) {
  portHandler->setPacketTimeout(1000.0);
  if (!portHandler->openPort()) {
    throw std::runtime_error("Failed to open the port!");
  }
}

std::string Dynamixel::modelNumber2Name(const uint16_t modelNumber) {
  switch (modelNumber) {
  case 29:
    return "MX-28";
  case 30:
    return "MX-28(2.0)";
  case 54:
    return "MX-64";
  case 311:
    return "MX-64(2.0)";
  default:
    return "Unknown";
  }
}

std::string Dynamixel::ping() const {
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
  return modelNumber2Name(modelNumber);
}

Dynamixel::~Dynamixel() { portHandler->closePort(); }
