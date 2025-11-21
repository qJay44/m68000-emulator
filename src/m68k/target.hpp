#pragma once

#include "types.hpp"

namespace m68k {

class Target {
public:
  enum class Kind : Byte {
    DataRegister,
    AddressRegister,
    Address,
    AddressPostincrement,
    AddressDecrement,
    AddressDisplacement,
    AddressIndex,
    ProgramCounterDisplacement,
    ProgramCounterIndex,
    AbsoluteShort,
    AbsoluteLong,
    Immeadiate
  };

private:
  Kind kind;
  Byte size;
  Byte index_;
  Word extWord0;
  Word extWord1;
  LongWord address;
};

} // namespace m68k

