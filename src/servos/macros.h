#ifndef MACROS_H
#define MACROS_H

#include <string>

#define MODEL_NAME(name)                                                       \
  static const std::string getName() { return name; }

#endif // MACROS_H
