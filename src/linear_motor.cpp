#define NULL nullptr
#include "linear_motor.h"
#include <AXL.h>
#include <AXM.h>
#include <cassert>
#include <iostream>

bool LinearMotor::isMotionModule() {
  unsigned long int result = 0;
  assert(AxmInfoIsMotionModule(&result) == 0);
  return result == 1;
}

double LinearMotor::getPosition() const {
  double position;
  AxmStatusGetActPos(axisNo, &position);
  return position;
}

bool LinearMotor::hasAlarm() const {
  unsigned long int alarm;
  AxmSignalReadServoAlarm(axisNo, &alarm);
  return alarm != 0;
}

void LinearMotor::resetAlarm() const {
  AxmSignalServoAlarmReset(axisNo, TRUE);
  while (hasAlarm())
    ;
  AxmSignalServoAlarmReset(axisNo, FALSE);
}

void LinearMotor::guessLimits() const {
  AxmMoveVel(axisNo, -100, 100, 100);
  while (!hasAlarm())
    ;
  resetAlarm();
  AxmStatusSetCmdPos(axisNo, 0);
  AxmStatusSetActPos(axisNo, 0);

  AxmMoveVel(axisNo, 100, 100, 100);
  while (!hasAlarm())
    ;
  resetAlarm();

  const double highLimit = getPosition();
  AxmStatusSetCmdPos(axisNo, highLimit);
  AxmMovePos(axisNo, 0, 100, 100, 100);

  AxmSignalSetSoftLimit(axisNo, ENABLE, EMERGENCY_STOP, COMMAND, highLimit - 1,
                        1);
  AxmMovePos(axisNo, highLimit, 100, 100, 100);
  AxmMovePos(axisNo, 0, 100, 100, 100);
}

LinearMotor::LinearMotor(const int axisNo) : axisNo(axisNo) {
  assert(AxlOpen(7) == 0);
  assert(isMotionModule());
  AxmMotSetPulseOutMethod(axisNo, TwoCcwCwHigh);             // 1
  AxmMotSetEncInputMethod(axisNo, ObverseSqr4Mode);          // 2
  AxmSignalSetInpos(axisNo, HIGH);                           // 3
  AxmSignalSetServoAlarm(axisNo, LOW);                       // 4
  AxmSignalSetLimit(axisNo, EMERGENCY_STOP, UNUSED, UNUSED); // 5, 6, TODO
  AxmMotSetMinVel(axisNo, 1);                                // 7
  AxmMotSetMaxVel(axisNo, 50);                               // 8, 700000
  AxmHomeSetMethod(axisNo, DIR_CCW, HomeSensor, LOW, 1000,
                   0);                              // 11, 9, 13, 22, 23
  AxmHomeSetSignalLevel(axisNo, LOW);               // 10
  AxmSignalSetZphaseLevel(axisNo, HIGH);            // 12
  AxmSignalSetStop(axisNo, EMERGENCY_STOP, UNUSED); // 14, 15
  AxmHomeSetVel(axisNo, 100, 100, 20, 1, 400, 400); // 16, 17, 18, 19, 20, 21
  AxmSignalSetSoftLimit(axisNo, DISABLE, EMERGENCY_STOP, COMMAND, 280000,
                        100);                        // 39, 38, 37, 24, 25
  AxmMotSetMoveUnitPerPulse(axisNo, 1, 2500);        // 27, 26
  AxmMotSetParaLoad(axisNo, 1000, 200, 400, 400);    // 28, 29, 30, 31
  AxmMotSetAbsRelMode(axisNo, POS_ABS_MODE);         // 32
  AxmMotSetProfileMode(axisNo, SYM_TRAPEZOIDE_MODE); // 33
  AxmSignalServoOn(axisNo, TRUE);                    // 34
  AxmSignalSetServoAlarmResetLevel(axisNo, HIGH);    // 35
  // 36: ENCODER_TYPE -> ENCODER_TYPE_INCREMENTAL
  AxmMotSetAccelUnit(axisNo, UNIT_SEC2); // 40

  if (hasAlarm()) {
    resetAlarm();
  }
}

LinearMotor::~LinearMotor() {
  AxmSignalServoOn(axisNo, FALSE);
  AxlClose();
}
