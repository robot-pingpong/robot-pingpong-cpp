#ifndef LINEAR_MOTOR_H
#define LINEAR_MOTOR_H

class LinearMotor {
  int axisNo;
  static bool isMotionModule();
  [[nodiscard]] double getPosition() const;

public:
  LinearMotor() = delete;
  explicit LinearMotor(int axisNo);

  [[nodiscard]] bool hasAlarm() const;
  void resetAlarm() const;
  void guessLimits() const;

  ~LinearMotor();
};

#endif // LINEAR_MOTOR_H
