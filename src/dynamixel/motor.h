#ifndef MOTOR_H
#define MOTOR_H

enum DriveMode {
  DIRECTION_CW = (0x00 << 0),
  DIRECTION_CCW = (0x01 << 0),
  PROFILE_VELOCITY = (0x00 << 2),
  PROFILE_TIME = (0x01 << 2),
};

enum Torque {
  ENABLE = 0x01,
  DISABLE = 0x00,
};

class Motor {
public:
  virtual ~Motor() = default;
  virtual void setTorqueEnable(Torque torque) = 0;
  virtual void setDriveMode(DriveMode mode) = 0;
  virtual void setGoalVelocity(unsigned int velocity) = 0;
  virtual void setProfileVelocity(unsigned int velocity) = 0;
  virtual void setProfileAcceleration(unsigned int acceleration) = 0;
};

#endif // MOTOR_H
