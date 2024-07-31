#include "arm.h"

#include "constants.h"

#include <cassert>
#include <iostream>
#include <thread>

Arm::Arm() {
  std::cout << base.ping() << std::endl;
  std::cout << pitchShoulder.ping() << std::endl;
  std::cout << yawShoulder.ping() << std::endl;
  std::cout << elbow.ping() << std::endl;
  std::cout << wrist.ping() << std::endl;

  std::cout << base.getAngle() << std::endl;
  std::cout << pitchShoulder.getAngle() << std::endl;
  std::cout << yawShoulder.getAngle() << std::endl;
  std::cout << elbow.getAngle() << std::endl;
  std::cout << wrist.getAngle() << std::endl;

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

  // base.setAngle(180);
  // yawShoulder.setAngle(180);
  // pitchShoulder.setAngle(230);
  // elbow.setAngle(180);
  // wrist.setAngle(180);
  std::vector<std::vector<double>> angle_set = {{260, 100, 230, 240, 90, 160},
                                                {260, 100, 223, 250, 90, 160},
                                                {250, 110, 206, 260, 100, 170},
                                                {240, 120, 189, 250, 110, 190},
                                                {220, 140, 172, 240, 150, 220}};
  for (const auto &angles : angle_set) {
    base.setAngle(angles[0]);
    yawShoulder.setAngle(angles[1]);
    pitchShoulder.setAngle(angles[2]);
    elbow.setAngle(angles[3]);
    wrist.setAngle(angles[4]);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    while (base.readMoving() || pitchShoulder.readMoving() ||
           yawShoulder.readMoving() || elbow.readMoving() || wrist.readMoving())
      ;
    wrist.setAngle(angles[5]);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}
