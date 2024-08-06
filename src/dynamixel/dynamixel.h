#ifndef SERVO_H
#define SERVO_H
#include "base_motor.h"

#include <dynamixel_sdk.h>
#include <map>
#include <string>

#define UNIT_SCALE 0.087891

static std::map<std::string, dynamixel::PortHandler *> controllers;
static dynamixel::PortHandler *getController(const std::string &portName);

namespace Servos {
template <typename Model> struct ControlTables {};

template <typename Model> class Dynamixel : public BaseMotor {
  dynamixel::PortHandler *portHandler;
  dynamixel::PacketHandler *packetHandler;

public:
  typedef typename Servos::ControlTables<Model>::protocol_t protocol_t;
  typename protocol_t::id_t id;
  void ping();
  bool reboot() override;
  double getAngle();
  void setAngle(double angle) override;
  const unsigned char readHardwareErrorStatus() override = 0;

protected:
  Dynamixel(const std::string &portName,
            typename Servos::ControlTables<Model>::Id_t id);
  void readByte(uint16_t address, int8_t &value);
  void readByte(uint16_t address, int16_t &value);
  void readByte(uint16_t address, int32_t &value);
  void readByte(uint16_t address, uint8_t &value);
  void readByte(uint16_t address, uint16_t &value);
  void readByte(uint16_t address, uint32_t &value);
  void writeByte(uint16_t address, int8_t value);
  void writeByte(uint16_t address, int16_t value);
  void writeByte(uint16_t address, int32_t value);
  void writeByte(uint16_t address, uint8_t value);
  void writeByte(uint16_t address, uint16_t value);
  void writeByte(uint16_t address, uint32_t value);
};
} // namespace Servos

#endif // SERVO_H
