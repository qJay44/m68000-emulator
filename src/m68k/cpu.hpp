#pragma once

#include <array>

#include "instructions.hpp"
#include "memory.hpp"
#include "types.hpp"

namespace m68k {

enum class Status {
  Init,
  Running,
  Stopped,
};

struct State {
  Status status;
  LongWord pc;               // Program Counter
  Word sr;                   // Status Register
  std::array<LongWord, 8> d; // Data Registers
  std::array<LongWord, 7> a; // Address Registers
  LongWord ssp;              // Supervisor Stack Pointer
  LongWord usp;              // User Stack Pointer
};

struct CPU {
  State state{};

  void init(Addressable& memory);
  void step(Addressable& memory);
  void execute(Addressable& memory, Instruction ins);

  Word readInstructionWord(Addressable& memory);
  Instruction decode(Addressable& memory);
};

}

