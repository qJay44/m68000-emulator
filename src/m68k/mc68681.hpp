#pragma once

#include "clock.hpp"
#include "memory.hpp"
#include "utils/types.hpp"

namespace m68k {

struct MC68681 : public Addressable, public Steppable {
  u8 port_a_status;
  u8 port_a_input;

  bool is_timing;
  u16 timer;
  u8 int_status;

  void read(Address addr, DataMutable data) override;
  void write(Address addr, DataImmutable data) override;

  ClockElapsed step(Clock clock) override;

  consteval size_t len() const;
};

}
