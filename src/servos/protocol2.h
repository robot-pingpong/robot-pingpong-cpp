#ifndef PROTOCOL2_H
#define PROTOCOL2_H
#include <cstdint>

namespace Protocols {
class Protocol2 {
public:
  static constexpr uint8_t version = 2;

  typedef uint8_t id_t;
  typedef uint8_t instr_t;
  typedef uint16_t address_t;
  typedef uint16_t length_t;

  static constexpr id_t broadcastId = 0xFE;

  struct Instr {
    static constexpr instr_t ping = 0x01;
    static constexpr instr_t read = 0x02;
    static constexpr instr_t write = 0x03;
    static constexpr instr_t regWrite = 0x04;
    static constexpr instr_t action = 0x05;
    static constexpr instr_t factoryReset = 0x06;
    static constexpr instr_t reboot = 0x08;
    static constexpr instr_t clear = 0x10;
    static constexpr instr_t controlTableBackup = 0x20;
    static constexpr instr_t syncRead = 0x82;
    static constexpr instr_t syncWrite = 0x83;
    static constexpr instr_t fastSyncRead = 0x8A;
    static constexpr instr_t bulkRead = 0x92;
    static constexpr instr_t bulkWrite = 0x93;
    static constexpr instr_t fastBulkRead = 0x9A;
  };
};
} // namespace Protocols

namespace Servos {
class BaseProtocol2 {
  typedef Protocols::Protocol2 protocol_t;
};
} // namespace Servos

#endif // PROTOCOL2_H
