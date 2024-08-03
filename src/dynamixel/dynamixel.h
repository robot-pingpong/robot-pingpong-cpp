#ifndef SERVO_H
#define SERVO_H
#include "motor.h"

#include <dynamixel_sdk.h>
#include <map>
#include <string>

#define UNIT_SCALE 0.087891

static std::map<std::string, dynamixel::PortHandler *> controllers;
static dynamixel::PortHandler *getController(const std::string &portName);

namespace Servos {
template <typename Model> struct ControlTables {};

template <typename Model> class Dynamixel : public Motor {
  dynamixel::PortHandler *portHandler;
  dynamixel::PacketHandler *packetHandler;

public:
  typedef typename Servos::ControlTables<Model>::protocol_t protocol_t;
  typename protocol_t::id_t id;
  void ping();
  bool reboot();
  double getAngle();
  void setAngle(double angle);

  void setTorqueEnable(typename Servos::ControlTables<Model>::TorqueEnable_t
                           torque) override = 0;
  void setTorqueEnable(const Torque torque) override {
    setTorqueEnable(
        static_cast<typename Servos::ControlTables<Model>::TorqueEnable_t>(
            torque));
  };
  void setDriveMode(
      typename Servos::ControlTables<Model>::DriveMode_t mode) override = 0;
  void setDriveMode(const DriveMode mode) override {
    setDriveMode(
        static_cast<typename Servos::ControlTables<Model>::DriveMode_t>(mode));
  };

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
