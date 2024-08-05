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
  for (const auto &motor : motors) {
    // motor->setGoalVelocity(400);
    // motor->setVelocityLimit(100);
    // motor->setAccelerationLimit(30);
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
  const auto target =
      std::ranges::find_if(angle_set, [z](const ArmDictionary &angle) {
        return z <= angle.maxHeight;
      });

  if (target == angle_set.end())
    return;

  try {
    base.setAngle(180);
    shoulder.setAngle(180);
    arm.setAngle(target->elbowAngle);
    elbow.setAngle(180);
    wrist.setAngle(180);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  resetted = false;
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
