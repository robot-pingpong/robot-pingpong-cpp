#ifndef TIMER_H
#define TIMER_H
#include <chrono>

class Timer {
  std::chrono::steady_clock::time_point prev;

public:
  Timer();
  double getFps();
};

#endif // TIMER_H
