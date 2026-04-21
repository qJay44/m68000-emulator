#pragma once

#include "utils/types.hpp"

namespace m68k {

using Clock = u64;
using ClockElapsed = u64; // The number of clock ticks that should elapsed before the next call to that device's step() method

struct Steppable {
  virtual ClockElapsed step(Clock clock) = 0;
};

}

