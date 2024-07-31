#ifndef DYNAMIXEL_H
#define DYNAMIXEL_H
#include "servos/mx28_p2.h"
#include "servos/servo.h"

#include <dynamixel_sdk.h>
#include <map>
#include <memory>
#include <string>

static std::map<std::string, dynamixel::PortHandler *> controllers;
static dynamixel::PortHandler *getController(const std::string &portName);

template <typename Motor> class Dynamixel {
  std::shared_ptr<dynamixel::PortHandler> controller;
  Motor motor;

public:
  Dynamixel(const std::string &portName,
            typename Servos::ControlTables<Motor>::id_t id);
  [[nodiscard]] std::string getName() const;
};

#endif // DYNAMIXEL_H
