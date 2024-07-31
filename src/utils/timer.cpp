#include "timer.h"

#include <chrono>

Timer::Timer() : prev(std::chrono::steady_clock::now()) {}

double Timer::getFps() {
  const auto curr = std::chrono::steady_clock::now();
  const auto elapsed = static_cast<double>(
      std::chrono::duration_cast<std::chrono::milliseconds>(curr - prev)
          .count());
  const auto fps = 1000.0 / elapsed;
  prev = curr;
  return fps;
}
