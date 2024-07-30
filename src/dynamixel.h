#ifndef DYNAMIXEL_H
#define DYNAMIXEL_H
#include <dynamixel_sdk.h>
#include <string>

class Dynamixel {
  dynamixel::PortHandler *portHandler;
  dynamixel::PacketHandler *packetHandler;
  int id;

  static std::string modelNumber2Name(uint16_t modelNumber);

public:
  explicit Dynamixel(const std::string &portName, int id);
  [[nodiscard]] std::string ping() const;
  ~Dynamixel();
};

#endif // DYNAMIXEL_H
