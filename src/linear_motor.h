#ifndef LINEAR_MOTOR_H
#define LINEAR_MOTOR_H

class LinearMotor {
  int axisNo;
  static bool isMotionModule();
  double min, max;

public:
  LinearMotor() = delete;
  explicit LinearMotor(int axisNo);

  [[nodiscard]] bool hasLimit() const;
  [[nodiscard]] double map(double value, double min, double max) const;
  [[nodiscard]] double getPosition() const;
  void setPosition(double position, bool wait = true) const;
  [[nodiscard]] bool hasAlarm() const;
  void resetAlarm() const;
  void guessLimits();

  ~LinearMotor();
};

#endif // LINEAR_MOTOR_H
