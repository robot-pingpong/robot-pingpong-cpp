#include "arm.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>

const std::vector<ArmDictionary> angle_set = {
    {0.2, 260, 100, 230, 240, 90, 160},
    {0.25, 260, 100, 223, 250, 90, 160},
    {0.32, 250, 110, 206, 260, 100, 170},
    {0.37, 240, 120, 189, 250, 110, 190},
    {0.48, 220, 140, 172, 240, 150, 220}};

Arm::Arm() {
  // base.setVelocityLimit(100);
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

  base.setGoalVelocity(100);
  base.setProfileVelocity(100);
  base.setProfileAcceleration(30);

  yawShoulder.setGoalVelocity(100);
  yawShoulder.setProfileVelocity(100);
  yawShoulder.setProfileAcceleration(30);

  pitchShoulder.setGoalVelocity(100);
  pitchShoulder.setProfileVelocity(100);
  pitchShoulder.setProfileAcceleration(30);

  elbow.setGoalVelocity(100);
  elbow.setProfileVelocity(100);
  elbow.setProfileAcceleration(30);

  wrist.setGoalVelocity(1000);
  wrist.setProfileVelocity(1200);
  wrist.setProfileAcceleration(300);

  base.setTorqueEnable(Torque::ENABLE);
  yawShoulder.setTorqueEnable(Torque::ENABLE);
  pitchShoulder.setTorqueEnable(Torque::ENABLE);
  elbow.setTorqueEnable(Torque::ENABLE);
  wrist.setTorqueEnable(Torque::ENABLE);

  moveByZ(0);
}

void Arm::moveByZ(const double z) {
  const auto target = std::find_if(
      angle_set.begin(), angle_set.end(),
      [z](const ArmDictionary &angle) { return z <= angle.maxHeight; });

  if (target == angle_set.end())
    return;

  base.setAngle(target->baseAngle);
  yawShoulder.setAngle(target->yawShoulderAngle);
  pitchShoulder.setAngle(target->pitchShoulderAngle);
  elbow.setAngle(target->elbowAngle);
  wrist.setAngle(target->wristBeforeAngle);
  //   std::this_thread::sleep_for(std::chrono::milliseconds(300));
  //   while (base.readMoving() || pitchShoulder.readMoving() ||
  //          yawShoulder.readMoving() || elbow.readMoving() ||
  //          wrist.readMoving())
  //     ;
  //   wrist.setAngle(angles[5]);
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
