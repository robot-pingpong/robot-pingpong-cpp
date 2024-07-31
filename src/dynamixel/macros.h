#ifndef MACROS_H
#define MACROS_H

enum DriveMode {
  DIRECTION_CW = (0x00 << 0),
  DIRECTION_CCW = (0x01 << 0),
  PROFILE_VELOCITY = (0x00 << 2),
  PROFILE_TIME = (0x01 << 2),
};

enum Torque {
  ENABLE = 0x01,
  DISABLE = 0x00,
};

#define MODEL_NAME(name)                                                       \
  static const std::string getName() { return name; }

#define READ_FIELD(Name)                                                       \
  const Servos::ControlTables<Model>::Name##_t read##Name() {                  \
    Servos::ControlTables<Model>::Name##_t value;                              \
    readByte(Servos::ControlTables<Model>::Name, value);                       \
    return value;                                                              \
  }

#define WRITE_FIELD(Name)                                                      \
  void set##Name(const Servos::ControlTables<Model>::Name##_t value) {         \
    writeByte(Servos::ControlTables<Model>::Name, value);                      \
  }

#define READ_WRITE_FIELD(Name)                                                 \
  READ_FIELD(Name)                                                             \
  WRITE_FIELD(Name)

#endif // MACROS_H
