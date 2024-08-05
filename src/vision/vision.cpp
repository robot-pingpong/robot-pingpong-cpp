#include "vision.h"

Vision::Vision() : t(screen){};

void Vision::init(Visualizer &visualizer, const bool skip) {
  t.setMask(skip);
  t.setTableArea(visualizer, skip);
}

bool Vision::track(cv::Vec3d &pos) {
  const bool lastSuccess = t.render(screen);
  if (lastSuccess) {
    pos = t.pos;
  }
  return lastSuccess;
}
