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

void Arm::init() {
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

  resetByZ(20);
}

bool Arm::inverseKinematics(const double x, const double y, const double z,
                            double &theta1, double &theta2, double &theta3,
                            const double pi) {
  const auto clampedZ = std::clamp(z, 20.0, 200.0);
  constexpr auto l1 = 198.251;
  constexpr auto l2 = 225;
  constexpr auto l3 = 30;
  const auto xn = clampedZ - l3 * std::cos(pi);
  const auto yn = x - l3 * std::sin(pi);
  const auto cosTheta2 =
      (xn * xn + yn * yn - l1 * l1 - l2 * l2) / (2 * l1 * l2);
  const auto sinTheta2 = std::sqrt(1 - cosTheta2 * cosTheta2);
  theta2 = std::atan2(sinTheta2, cosTheta2);
  const auto k1 = l1 + l2 * cosTheta2;
  const auto k2 = l2 * sinTheta2;
  theta1 = std::atan2(k2, k1) - std::atan2(yn, xn);
  theta3 = pi + theta1 - theta2;

  theta1 = theta1 / M_PI * 180 + 180 + 43.7;
  theta2 = theta2 / M_PI * 180 - 70 + 180;
  theta3 = theta3 / M_PI * 180 + 180;

  if (std::isnan(theta1) || std::isnan(theta2) || std::isnan(theta3)) {
    return false;
  }
  return true;
}

void Arm::move(const double y, const double z, const bool hitTarget) {
  std::thread([&, y, z, hitTarget] {
    if (!mtx.try_lock()) {
      return;
    }
    try {
      for (;;) {
        double theta1, theta2, theta3;
        if (!inverseKinematics(hitTarget ? 210 : 190, 0, z, theta1, theta2,
                               theta3, hitTarget ? M_PI / 3 : M_PI / 2)) {
          break;
        }
        base.setAngle(
            std::clamp((y - (Y_TABLE_SIZE / 2)) * 20 + 180, 150.0, 210.0));
        shoulder.setAngle(theta1);
        arm.setAngle(200);
        elbow.setAngle(theta2);
        wrist.setAngle(theta3 - (hitTarget ? 60 : 0));
        break;
      }
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
    resetted = false;
    mtx.unlock();
  }).detach();
}

void Arm::resetByZ(const double z) {
  if (resetted)
    return;
  std::thread([&, z] {
    mtx.lock();
    try {
      for (;;) {
        double theta1, theta2, theta3;
        if (!inverseKinematics(190, 0, z, theta1, theta2, theta3)) {
          break;
        }
        base.setAngle(180);
        shoulder.setAngle(theta1);
        arm.setAngle(200);
        elbow.setAngle(theta2);
        wrist.setAngle(theta3);
        break;
      }
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
    mtx.unlock();
  }).detach();
  resetted = true;
}
