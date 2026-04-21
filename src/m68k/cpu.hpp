#pragma once

#include "memory.hpp"
#include "state.hpp"

namespace m68k {

struct CPU {
  State state{};

  void init(Addressable& memory);
  void step(Addressable& memory);
};

}

