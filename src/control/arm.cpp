#include "arm.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>
#include <valarray>

void Arm::init() {
  for (const auto &motor : motors) {
    // motor->setGoalVelocity(400);
    // motor->setVelocityLimit(100);
    // motor->setAccelerationLimit(30);
    if (motor->readHardwareErrorStatus() != 0) {
      motor->reboot();
    }

    motor->setProfileVelocity(0);
    motor->setProfileAcceleration(0);
    motor->setTorqueEnable(Torque::ENABLE);
  }
  base.setPositionPGain(400);
  shoulder.setPositionPGain(400);

  // wrist.setGoalVelocity(1000);
  pitchWrist.setProfileVelocity(1800);
  pitchWrist.setProfileAcceleration(450);

  std::thread([&] {
    resetByZ(50);
    resetted = false;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    resetByZ(400);
    resetted = false;
  }).detach();
}

bool Arm::inverseKinematics(const double x, const double y, const double z,
                            double &yawTheta, double &theta1, double &theta2,
                            double &theta3, const double pi) {
  yawTheta = std::atan2(y, x);
  const auto xPrime = std::sqrt(x * x + y * y);
  const auto clampedZ = std::clamp(z, 20.0, 390.0);
  constexpr auto l1 = 188.172;
  constexpr auto l2 = 231.018;
  constexpr auto l3 = 30;
  const auto xn = clampedZ - l3 * std::cos(pi);
  const auto yn = xPrime - l3 * std::sin(pi);
  const auto cosTheta2 =
      (xn * xn + yn * yn - l1 * l1 - l2 * l2) / (2 * l1 * l2);
  const auto sinTheta2 = std::sqrt(1 - cosTheta2 * cosTheta2);
  theta2 = std::atan2(sinTheta2, cosTheta2);
  const auto k1 = l1 + l2 * cosTheta2;
  const auto k2 = l2 * sinTheta2;
  theta1 = std::atan2(k2, k1) - std::atan2(yn, xn);
  theta3 = pi + theta1 - theta2;
  yawTheta = 180 + yawTheta / M_PI * 180;
  theta1 = 180 + theta1 / M_PI * 180 + 25.0;
  theta2 = theta2 / M_PI * 180 + 180 - (90 - 25.0) - 10.8;
  theta3 = 180 + 10.8 + theta3 / M_PI * 180;

  if (std::isnan(yawTheta) || std::isnan(theta1) || std::isnan(theta2) ||
      std::isnan(theta3)) {
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
        double yawTheta, theta1, theta2, theta3;
        int maxX = 320;
        for (; maxX > 120 &&
               !inverseKinematics(hitTarget ? maxX : 120, 0,
                                  z + (hitTarget ? 40 : 0), yawTheta, theta1,
                                  theta2, theta3,
                                  (hitTarget ? 60 : 100) * M_PI / 180);
             --maxX)
          ;
        if (maxX <= 120) {
          break;
        }
        auto writer = base.getBulkWriter();
        base.setAngleBulk(writer, yawTheta);
        shoulder.setAngleBulk(writer, theta1);
        elbow.setAngleBulk(writer, theta2);
        yawWrist.setAngleBulk(writer, 360 - yawTheta);
        pitchWrist.setAngleBulk(writer, std::clamp(theta3, 100.0, 230.0));
        if (const int result = writer.txPacket(); result != COMM_SUCCESS) {
          std::cerr << dynamixel::PacketHandler::getPacketHandler()
                           ->getTxRxResult(result)
                    << std::endl;
          throw std::runtime_error("Failed to send bulk write packet");
        }
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
    if (!mtx.try_lock()) {
      return;
    }
    try {
      for (;;) {
        double yawTheta, theta1, theta2, theta3;
        if (!inverseKinematics(120, 0, z, yawTheta, theta1, theta2, theta3)) {
          break;
        }
        base.setAngle(yawTheta);
        shoulder.setAngle(theta1);
        elbow.setAngle(theta2);
        yawWrist.setAngle(360 - yawTheta);
        pitchWrist.setAngle(theta3);
        resetted = true;
        break;
      }
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
    mtx.unlock();
  }).detach();
}
