#include "arm.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>

const std::vector<ArmDictionary> angle_set = {
    {0.20, 160, 100, 230, 240, 90, 160},
    {0.25, 170, 100, 223, 250, 90, 160},
    {0.32, 180, 110, 206, 260, 100, 170},
    {0.37, 190, 120, 189, 250, 110, 190},
    {0.48, 200, 140, 172, 240, 150, 220}};

Arm::Arm() {
  // base.setTorqueEnable(Torque::DISABLE);
  // yawShoulder.setTorqueEnable(Torque::DISABLE);
  // pitchShoulder.setTorqueEnable(Torque::DISABLE);
  // elbow.setTorqueEnable(Torque::DISABLE);
  // wrist.setTorqueEnable(Torque::DISABLE);
  // base.setVelocityLimit(400);
  // yawShoulder.setVelocityLimit(400);
  // pitchShoulder.setVelocityLimit(400);
  // elbow.setVelocityLimit(400);
  // wrist.setVelocityLimit(400);
  // base.setTorqueEnable(Torque::ENABLE);
  // yawShoulder.setTorqueEnable(Torque::ENABLE);
  // pitchShoulder.setTorqueEnable(Torque::ENABLE);
  // elbow.setTorqueEnable(Torque::ENABLE);
  // wrist.setTorqueEnable(Torque::ENABLE);

  // base.setAccelerationLimit(30);
  // yawShoulder.setDriveMode(DriveMode::DIRECTION_CCW |
  //                          DriveMode::PROFILE_VELOCITY);
  // yawShoulder.setVelocityLimit(100);
  // yawShoulder.setAccelerationLimit(30);
  // pitchShoulder.setVelocityLimit(100);
  // pitchShoulder.setAccelerationLimit(30);
  // elbow.setVelocityLimit(100);
  // elbow.setAccelerationLimit(30);
  // wrist.setVelocityLimit(1000);
  // wrist.setAccelerationLimit(300);

  for (const auto &motor : motors) {
    // motor->setGoalVelocity(400);
    motor->setProfileVelocity(400);
    motor->setProfileAcceleration(120);
    motor->setTorqueEnable(Torque::ENABLE);
  }

  // wrist.setGoalVelocity(1000);
  // wrist.setProfileVelocity(1800);
  // wrist.setProfileAcceleration(450);

  moveByZ(0);
}

void Arm::moveByZ(const double z) {
  const auto target = std::find_if(
      angle_set.begin(), angle_set.end(),
      [z](const ArmDictionary &angle) { return z <= angle.maxHeight; });

  if (target == angle_set.end())
    return;

  try {
    base.setAngle(target->baseAngle);
    shoulder.setAngle(200);
    // yawShoulder.setAngle(target->yawShoulderAngle);
    // pitchShoulder.setAngle(target->pitchShoulderAngle);
    // elbow.setAngle(target->elbowAngle);
    // wrist.setAngle(target->wristBeforeAngle);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  resetted = false;
}

void Arm::hitByZ(const double z) {
  const auto target = std::find_if(
      angle_set.begin(), angle_set.end(),
      [z](const ArmDictionary &angle) { return z <= angle.maxHeight; });

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
  base.setAngle(angle_set[2].baseAngle);
  // yawShoulder.setAngle(angle_set[2].yawShoulderAngle);
  // pitchShoulder.setAngle(angle_set[2].pitchShoulderAngle);
  // elbow.setAngle(angle_set[2].elbowAngle);
  // wrist.setAngle(angle_set[2].wristBeforeAngle);
  resetted = true;
}
