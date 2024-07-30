#ifndef LINEAR_MOTOR_H
#define LINEAR_MOTOR_H

class LinearMotor {
  int axisNo;
  static bool isMotionModule();

public:
  LinearMotor() = delete;
  explicit LinearMotor(int axisNo);

  [[nodiscard]] bool hasAlarm() const;
  void resetAlarm() const;

  ~LinearMotor();
};

#endif // LINEAR_MOTOR_H
