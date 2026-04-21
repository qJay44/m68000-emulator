#pragma once

#include "memory.hpp"
#include "state.hpp"

namespace m68k {

struct Context {
  State& state;
  Addressable& memory;
};

}

