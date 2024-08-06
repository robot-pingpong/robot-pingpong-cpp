#ifndef MOTOR_H
#define MOTOR_H

enum class DriveMode : unsigned char {
  DIRECTION_CW = (0x00 << 0),
  DIRECTION_CCW = (0x01 << 0),
  PROFILE_VELOCITY = (0x00 << 2),
  PROFILE_TIME = (0x01 << 2),
};

enum class Torque : unsigned char {
  ENABLE = 0x01,
  DISABLE = 0x00,
};

enum class HardwareError : unsigned char {
  INPUT_VOLTAGE = 1 << 0,
  OVERHEATING = 1 << 2,
  MOTOR_ENCODER = 1 << 3,
  ELECTRONICAL_SHOCK = 1 << 4,
  OVERLOAD = 1 << 5,
};

class BaseMotor {
public:
  virtual ~BaseMotor() = default;
  virtual void setTorqueEnable(unsigned char torque) = 0;
  virtual void setTorqueEnable(Torque torque) {
    setTorqueEnable(static_cast<unsigned char>(torque));
  }
  virtual void setDriveMode(unsigned char mode) = 0;
  virtual void setDriveMode(DriveMode mode) {
    setDriveMode(static_cast<unsigned char>(mode));
  }
  virtual void setGoalVelocity(unsigned int velocity) = 0;
  virtual void setProfileVelocity(unsigned int velocity) = 0;
  virtual void setProfileAcceleration(unsigned int acceleration) = 0;
  virtual void setAngle(double angle) = 0;
  virtual const unsigned char // NOLINT(*-const-return-type)
  readHardwareErrorStatus() = 0;
  virtual bool reboot() = 0;
};

#endif // MOTOR_H
