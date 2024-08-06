#include "arm.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>
#include <valarray>

const std::vector<ArmDictionary> angle_set = {
    {0.20, 160, 100, 230, 240, 90, 160},
    {0.25, 170, 100, 223, 250, 90, 160},
    {0.32, 180, 110, 206, 260, 100, 170},
    {0.37, 190, 120, 189, 250, 110, 190},
    {0.48, 200, 140, 172, 240, 150, 220}};

Arm::Arm() {
  for (const auto &motor : motors) {
    // motor->setGoalVelocity(400);
    // motor->setVelocityLimit(100);
    // motor->setAccelerationLimit(30);
    if (motor->readHardwareErrorStatus() != 0) {
      motor->reboot();
    }

    motor->setProfileVelocity(400);
    motor->setProfileAcceleration(120);
    motor->setTorqueEnable(Torque::ENABLE);
  }
  arm.setAngle(200);

  // wrist.setGoalVelocity(1000);
  wrist.setProfileVelocity(1800);
  wrist.setProfileAcceleration(450);

  moveByZ(100);
}

void Arm::moveByZ(const double z) {
  if (z > 400 || z < 0)
    return;
  constexpr auto l1 = 198.251;
  constexpr auto l2 = 225;
  constexpr auto l3 = 30;
  constexpr auto pi = M_PI / 2;
  const auto x = z;
  constexpr auto y = 190;
  const auto xn = x - l3 * std::cos(pi);
  const auto yn = y - l3 * std::sin(pi);
  const auto cosTheta2 =
      (xn * xn + yn * yn - l1 * l1 - l2 * l2) / (2 * l1 * l2);
  const auto sinTheta2 = std::sqrt(1 - cosTheta2 * cosTheta2);
  const auto theta2 = std::atan2(sinTheta2, cosTheta2);
  const auto k1 = l1 + l2 * cosTheta2;
  const auto k2 = l2 * sinTheta2;
  const auto theta1 = std::atan2(k2, k1) - std::atan2(yn, xn);
  const auto theta3 = pi + theta1 - theta2;

  shoulder.setAngle(theta1 / M_PI * 180 + 180 + 43.7);
  arm.setAngle(200);
  elbow.setAngle(theta2 / M_PI * 180 - 70 + 180);
  wrist.setAngle(theta3 / M_PI * 180 + 180);
}

void Arm::hitByZ(const double z) {
  const auto target =
      std::ranges::find_if(angle_set, [z](const ArmDictionary &angle) {
        return z <= angle.maxHeight;
      });

  if (target == angle_set.end())
    return;

  // while (
  //     // base.readMoving() || pitchShoulder.readMoving() ||
  //     //      yawShoulder.readMoving() || elbow.readMoving() ||
  //     wrist.readMoving())
  //   ;
  // wrist.setAngle(target->wristAfterAngle);
  resetted = false;
}

void Arm::resetByZ(const double z) {
  if (resetted)
    return;
  // pitchShoulder.setAngle(angle_set[2].pitchShoulderAngle);
  // elbow.setAngle(angle_set[2].elbowAngle);
  // wrist.setAngle(angle_set[2].wristBeforeAngle);
  resetted = true;
}
