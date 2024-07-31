#ifndef SERVO_H
#define SERVO_H

namespace Servos {
template <typename Model> struct ControlTables {};

template <typename Model> class Servo {
public:
  typedef typename Servos::ControlTables<Model>::protocol_t protocol_t;
  typename protocol_t::id_t id;

protected:
  explicit Servo(typename protocol_t::id_t id) : id(id) {}
};
} // namespace Servos

#endif // SERVO_H
