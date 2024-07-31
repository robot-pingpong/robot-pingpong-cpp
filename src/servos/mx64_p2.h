#ifndef MX64_P2_H
#define MX64_P2_H
#include "protocol2.h"
#include "servo.h"

namespace Servos {
class Mx64P2;

template <> struct ControlTables<Mx64P2> {
  typedef Protocols::Protocol2 protocol_t;
  static constexpr protocol_t::address_t modelNumber = 0;
  typedef uint16_t modelNumber_t;
  static constexpr modelNumber_t modelNumberValue = 311;

  static constexpr protocol_t::address_t modelInfo = 2;
  typedef uint32_t modelInfo_t;
  static constexpr protocol_t::address_t firmwareVersion = 6;
  typedef uint8_t firmwareVersion_t;
  static constexpr protocol_t::address_t id = 7;
  typedef protocol_t::id_t id_t;
  static constexpr protocol_t::address_t baudrate = 8;
  typedef uint8_t baudrate_t;
  static constexpr protocol_t::address_t returnDelayTime = 9;
  typedef uint8_t returnDelayTime_t;
  static constexpr protocol_t::address_t driveMode = 10;
  typedef uint8_t driveMode_t;
  static constexpr protocol_t::address_t operatingMode = 11;
  typedef uint8_t operatingMode_t;
  static constexpr protocol_t::address_t secondaryId = 12;
  typedef uint8_t secondaryId_t;
  static constexpr protocol_t::address_t protocolVersion = 13;
  typedef uint8_t protocolVersion_t;
  static constexpr protocol_t::address_t homingOffset = 20;
  typedef uint32_t homingOffset_t;
  static constexpr protocol_t::address_t movingThreshold = 24;
  typedef uint32_t movingThreshold_t;
  static constexpr protocol_t::address_t highestTemperatureLimit = 31;
  typedef uint8_t highestTemperatureLimit_t;
  static constexpr protocol_t::address_t highestVoltageLimit = 32;
  typedef uint16_t highestVoltageLimit_t;
  static constexpr protocol_t::address_t lowestVoltageLimit = 34;
  typedef uint16_t lowestVoltageLimit_t;
  static constexpr protocol_t::address_t pwmLimit = 36;
  typedef uint16_t pwmLimit_t;
  static constexpr protocol_t::address_t currentLimit = 38;
  typedef uint16_t currentLimit_t;
  static constexpr protocol_t::address_t accelerationLimit = 40;
  typedef uint32_t accelerationLimit_t;
  static constexpr protocol_t::address_t speedLimit = 44;
  typedef uint32_t speedLimit_t;
  static constexpr protocol_t::address_t maxPositionLimit = 48;
  typedef int32_t maxPositionLimit_t;
  static constexpr protocol_t::address_t minPositionLimit = 52;
  typedef int32_t minPositionLimit_t;
  static constexpr protocol_t::address_t alarmShutdown = 63;
  typedef uint8_t alarmShutdown_t;

  static constexpr protocol_t::address_t torqueEnable = 64;
  typedef uint8_t torqueEnable_t;
  static constexpr protocol_t::address_t led = 65;
  typedef uint8_t led_t;
  static constexpr protocol_t::address_t statusReturnLevel = 68;
  typedef uint8_t statusReturnLevel_t;
  static constexpr protocol_t::address_t registered = 69;
  typedef uint8_t registered_t;
  static constexpr protocol_t::address_t hardwareErrorStatus = 70;
  typedef uint8_t hardwareErrorStatus_t;
  static constexpr protocol_t::address_t velocityIGain = 76;
  typedef uint16_t velocityIGain_t;
  static constexpr protocol_t::address_t velocityPGain = 78;
  typedef uint16_t velocityPGain_t;
  static constexpr protocol_t::address_t positionDGain = 80;
  typedef uint16_t positionDGain_t;
  static constexpr protocol_t::address_t positionIGain = 82;
  typedef uint16_t positionIGain_t;
  static constexpr protocol_t::address_t positionPGain = 84;
  typedef uint16_t positionPGain_t;
  static constexpr protocol_t::address_t feedforward2ndGain = 88;
  typedef uint16_t feedforward2ndGain_t;
  static constexpr protocol_t::address_t feedforward1stGain = 90;
  typedef uint16_t feedforward1stGain_t;
  static constexpr protocol_t::address_t busWatchdog = 98;
  typedef uint8_t busWatchdog_t;
  static constexpr protocol_t::address_t goalPwm = 100;
  typedef uint16_t goalPwm_t;
  static constexpr protocol_t::address_t goalCurrent = 102;
  typedef uint16_t goalCurrent_t;
  static constexpr protocol_t::address_t movingSpeed = 104;
  typedef uint32_t movingSpeed_t;
  static constexpr movingSpeed_t minGoalSpeed = INT32_MIN;
  static constexpr movingSpeed_t maxGoalSpeed = INT32_MAX;
  static constexpr double rpmPerTick = 0.229;
  static constexpr bool speedSignBit = false;
  static constexpr protocol_t::address_t profileAcceleration = 108;
  typedef uint32_t profileAcceleration_t;
  static constexpr protocol_t::address_t profileSpeed = 112;
  typedef uint32_t profileSpeed_t;
  static constexpr protocol_t::address_t goalPosition = 116;
  typedef int32_t goalPosition_t;
  static constexpr goalPosition_t minGoalPosition = 0;
  static constexpr goalPosition_t maxGoalPosition = 4095;
  static constexpr uint32_t minGoalAngleDeg = 0;
  static constexpr uint32_t maxGoalAngleDeg = 360;
  static constexpr protocol_t::address_t realtimeTick = 120;
  typedef uint16_t realtimeTick_t;
  static constexpr protocol_t::address_t moving = 122;
  typedef uint8_t moving_t;
  static constexpr protocol_t::address_t movingStatus = 123;
  typedef uint8_t movingStatus_t;
  static constexpr protocol_t::address_t presentPwm = 124;
  typedef uint16_t presentPwm_t;
  static constexpr protocol_t::address_t presentCurrent = 126;
  typedef uint16_t presentCurrent_t;
  static constexpr protocol_t::address_t presentSpeed = 128;
  typedef int32_t presentSpeed_t;
  static constexpr protocol_t::address_t presentPosition = 132;
  typedef int32_t presentPosition_t;
  static constexpr protocol_t::address_t speedTrajectory = 136;
  typedef int32_t speedTrajectory_t;
  static constexpr protocol_t::address_t positionTrajectory = 140;
  typedef int32_t positionTrajectory_t;
  static constexpr protocol_t::address_t presentVoltage = 144;
  typedef uint16_t presentVoltage_t;
  static constexpr protocol_t::address_t presentTemperature = 146;
  typedef uint8_t presentTemperature_t;
};

class Mx64P2 : public Servo<Mx64P2> {
public:
  typedef Mx64P2 Model;
  explicit Mx64P2(const protocol_t::id_t id) : Servo(id){};
};
} // namespace Servos

#endif // MX64_P2_H
