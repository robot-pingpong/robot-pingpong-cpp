#ifndef MACROS_H
#define MACROS_H

#include <string>

#define MODEL_NAME(name)                                                       \
  static const std::string getName() { return name; }

#define READ_FIELD(Name)                                                       \
  const Servos::ControlTables<Model>::##Name##_t read##Name() {                \
    Servos::ControlTables<Model>::##Name##_t value;                            \
    readByte(Servos::ControlTables<Model>::##Name, value);                     \
    return value;                                                              \
  }

#endif // MACROS_H
