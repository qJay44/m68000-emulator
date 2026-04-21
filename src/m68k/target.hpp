#pragma once

#include "types.hpp"

namespace m68k {

struct Target {
  enum class Kind : Byte {
    No,
    DataRegister,
    AddressRegister,
    Address,
    AddressPostincrement,
    AddressPredecrement,
    AddressDisplacement,
    AddressIndex,
    ProgramCounterDisplacement,
    ProgramCounterIndex,
    AbsoluteShort,
    AbsoluteLong,
    Immeadiate
  };

  Kind kind;
  Byte size;
  Byte index;
  Word extWord0;
  Word extWord1;
  LongWord address;
};

} // namespace m68k

