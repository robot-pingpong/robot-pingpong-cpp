#ifndef MX28_P2_H
#define MX28_P2_H
#include "dynamixel.h"
#include "macros.h"
#include "protocol2.h"

namespace Servos {
class Mx28P2;

template <> struct ControlTables<Mx28P2> {
  typedef Protocols::Protocol2 protocol_t;
  static constexpr protocol_t::address_t ModelNumber = 0;
  typedef uint16_t modelNumber_t;
  static constexpr modelNumber_t modelNumberValue = 30;

  static constexpr protocol_t::address_t ModelInfo = 2;
  typedef uint32_t ModelInfo_t;
  static constexpr protocol_t::address_t FirmwareVersion = 6;
  typedef uint8_t FirmwareVersion_t;
  static constexpr protocol_t::address_t Id = 7;
  typedef protocol_t::id_t id_t;
  static constexpr protocol_t::address_t Baudrate = 8;
  typedef uint8_t Baudrate_t;
  static constexpr protocol_t::address_t ReturnDelayTime = 9;
  typedef uint8_t ReturnDelayTime_t;
  static constexpr protocol_t::address_t DriveMode = 10;
  typedef uint8_t DriveMode_t;
  static constexpr protocol_t::address_t OperatingMode = 11;
  typedef uint8_t OperatingMode_t;
  static constexpr protocol_t::address_t SecondaryId = 12;
  typedef uint8_t SecondaryId_t;
  static constexpr protocol_t::address_t ProtocolVersion = 13;
  typedef uint8_t ProtocolVersion_t;
  static constexpr protocol_t::address_t HomingOffset = 20;
  typedef uint32_t HomingOffset_t;
  static constexpr protocol_t::address_t MovingThreshold = 24;
  typedef uint32_t MovingThreshold_t;
  static constexpr protocol_t::address_t HighestTemperatureLimit = 31;
  typedef uint8_t HighestTemperatureLimit_t;
  static constexpr protocol_t::address_t HighestVoltageLimit = 32;
  typedef uint16_t HighestVoltageLimit_t;
  static constexpr protocol_t::address_t LowestVoltageLimit = 34;
  typedef uint16_t LowestVoltageLimit_t;
  static constexpr protocol_t::address_t PwmLimit = 36;
  typedef uint16_t PwmLimit_t;
  static constexpr protocol_t::address_t CurrentLimit = 38;
  typedef uint16_t CurrentLimit_t;
  static constexpr protocol_t::address_t AccelerationLimit = 40;
  typedef uint32_t AccelerationLimit_t;
  static constexpr protocol_t::address_t VelocityLimit = 44;
  typedef uint32_t VelocityLimit_t;
  static constexpr protocol_t::address_t MaxPositionLimit = 48;
  typedef int32_t MaxPositionLimit_t;
  static constexpr protocol_t::address_t MinPositionLimit = 52;
  typedef int32_t MinPositionLimit_t;
  static constexpr protocol_t::address_t AlarmShutdown = 63;
  typedef uint8_t AlarmShutdown_t;

  static constexpr protocol_t::address_t TorqueEnable = 64;
  typedef uint8_t TorqueEnable_t;
  static constexpr protocol_t::address_t Led = 65;
  typedef uint8_t Led_t;
  static constexpr protocol_t::address_t StatusReturnLevel = 68;
  typedef uint8_t StatusReturnLevel_t;
  static constexpr protocol_t::address_t Registered = 69;
  typedef uint8_t Registered_t;
  static constexpr protocol_t::address_t HardwareErrorStatus = 70;
  typedef uint8_t HardwareErrorStatus_t;
  static constexpr protocol_t::address_t VelocityIGain = 76;
  typedef uint16_t VelocityIGain_t;
  static constexpr protocol_t::address_t VelocityPGain = 78;
  typedef uint16_t VelocityPGain_t;
  static constexpr protocol_t::address_t PositionDGain = 80;
  typedef uint16_t PositionDGain_t;
  static constexpr protocol_t::address_t PositionIGain = 82;
  typedef uint16_t PositionIGain_t;
  static constexpr protocol_t::address_t PositionPGain = 84;
  typedef uint16_t PositionPGain_t;
  static constexpr protocol_t::address_t Feedforward2ndGain = 88;
  typedef uint16_t Feedforward2ndGain_t;
  static constexpr protocol_t::address_t Feedforward1stGain = 90;
  typedef uint16_t Feedforward1stGain_t;
  static constexpr protocol_t::address_t BusWatchdog = 98;
  typedef uint8_t BusWatchdog_t;
  static constexpr protocol_t::address_t GoalPwm = 100;
  typedef uint16_t GoalPwm_t;
  static constexpr protocol_t::address_t GoalCurrent = 102;
  typedef uint16_t GoalCurrent_t;
  static constexpr protocol_t::address_t GoalVelocity = 104;
  typedef uint32_t GoalVelocity_t;
  static constexpr GoalVelocity_t minGoalVelocity = -1023;
  static constexpr GoalVelocity_t maxGoalVelocity = 1023;
  static constexpr double rpmPerTick = 0.229;
  static constexpr bool speedSignBit = false;
  static constexpr protocol_t::address_t ProfileAcceleration = 108;
  typedef uint32_t ProfileAcceleration_t;
  static constexpr protocol_t::address_t ProfileSpeed = 112;
  typedef uint32_t ProfileSpeed_t;
  static constexpr protocol_t::address_t GoalPosition = 116;
  typedef int32_t GoalPosition_t;
  static constexpr GoalPosition_t minGoalPosition = 0;
  static constexpr GoalPosition_t maxGoalPosition = 4095;
  static constexpr uint32_t minGoalAngleDeg = 0;
  static constexpr uint32_t maxGoalAngleDeg = 360;
  static constexpr protocol_t::address_t RealtimeTick = 120;
  typedef uint16_t RealtimeTick_t;
  static constexpr protocol_t::address_t Moving = 122;
  typedef uint8_t Moving_t;
  static constexpr protocol_t::address_t MovingStatus = 123;
  typedef uint8_t MovingStatus_t;
  static constexpr protocol_t::address_t PresentPwm = 124;
  typedef uint16_t PresentPwm_t;
  static constexpr protocol_t::address_t PresentCurrent = 126;
  typedef uint16_t PresentCurrent_t;
  static constexpr protocol_t::address_t PresentSpeed = 128;
  typedef int32_t PresentSpeed_t;
  static constexpr protocol_t::address_t PresentPosition = 132;
  typedef int32_t PresentPosition_t;
  static constexpr protocol_t::address_t SpeedTrajectory = 136;
  typedef int32_t SpeedTrajectory_t;
  static constexpr protocol_t::address_t PositionTrajectory = 140;
  typedef int32_t PositionTrajectory_t;
  static constexpr protocol_t::address_t PresentVoltage = 144;
  typedef uint16_t PresentVoltage_t;
  static constexpr protocol_t::address_t PresentTemperature = 146;
  typedef uint8_t PresentTemperature_t;
};

class Mx28P2 : public Dynamixel<Mx28P2> {
public:
  typedef Mx28P2 Model;
  explicit Mx28P2(const std::string &portName, const protocol_t::id_t id)
      : Dynamixel(portName, id){};

  MODEL_NAME("MX-28(2.0)")

  READ_WRITE_FIELD(Led)
  READ_WRITE_FIELD(VelocityLimit)
  READ_FIELD(PresentPosition)
};
} // namespace Servos

#endif // MX28_P2_H
