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

    motor->setProfileVelocity(400);
    motor->setProfileAcceleration(120);
    motor->setTorqueEnable(Torque::ENABLE);
  }
  base.setPositionPGain(400);
  shoulder.setPositionDGain(500);

  // wrist.setGoalVelocity(1000);
  wrist.setProfileVelocity(1800);
  wrist.setProfileAcceleration(450);

  std::thread([&] {
    resetByZ(20);
    resetted = false;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    resetByZ(400);
    resetted = false;
  }).detach();
}

bool Arm::inverseKinematics(const double x, const double y, const double z,
                            double &theta1, double &theta2, double &theta3,
                            const double pi) {
  const auto clampedZ = std::clamp(z, 20.0, 390.0);
  constexpr auto l1 = 187.375;
  constexpr auto l2 = 209.90;
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

  theta1 = theta1 / M_PI * 180 + 180 + 24.9;
  theta2 = theta2 / M_PI * 180 - (90 - 24.9) + 180;
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
        if (!inverseKinematics(120, 0, z, theta1, theta2, theta3,
                               120 * M_PI / 180)) {
          break;
        }
        auto writer = base.getBulkWriter();
        base.setAngleBulk(
            writer,
            std::clamp((y - (Y_TABLE_SIZE / 2)) * 20 + 180, 150.0, 210.0));
        shoulder.setAngleBulk(writer, theta1 - (hitTarget ? 50 : 0));
        elbow.setAngleBulk(writer, theta2 - (hitTarget ? 45 : 0));
        wrist.setAngleBulk(writer, theta3 - (hitTarget ? 50 : 0));
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
        double theta1, theta2, theta3;
        if (!inverseKinematics(120, 0, z, theta1, theta2, theta3)) {
          break;
        }
        base.setAngle(180);
        shoulder.setAngle(theta1);
        elbow.setAngle(theta2);
        wrist.setAngle(theta3);
        resetted = true;
        break;
      }
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
    mtx.unlock();
  }).detach();
}
