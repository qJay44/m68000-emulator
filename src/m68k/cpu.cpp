#include "cpu.hpp"

#include <cstdlib>
#include <print>

#include "instructions.hpp"
#include "types.hpp"
#include "utils/utils.hpp"

namespace m68k {

void CPU::init(Addressable& memory) {
  state.ssp = memory.read<LongWord>(0);
  state.pc = memory.read<LongWord>(4);
  state.status = Status::Running;
  exit(0);
}

void CPU::step(Addressable& memory) {
  switch (state.status) {
    case Status::Init:
      init(memory);
      break;
    case Status::Running: {
      LongWord addr = state.pc;
      Instruction ins = decode(memory);
      std::println("pc: {:#x}, instruction: {}", addr, (Word)ins);
      execute(memory, ins);
      break;
    }
    case Status::Stopped:
      warning("CPU has Stopped");
      break;
  }

}

void CPU::execute(Addressable& memory, Instruction ins) {
  switch (ins) {
    case Instruction::NOP:
      break;
    default:
      error("[m68k::CPU::execute] Trying to execute unimplemented instruction [0x{:#x}]", (Word)ins);
  }
}

Word CPU::readInstructionWord(Addressable& memory) {
  Word ins = memory.read<Word>(state.pc);
  state.pc += 2;

  return ins;
}

Instruction CPU::decode(Addressable& memory) {
  Word ins = readInstructionWord(memory);

  switch (ins) {
    case 0x4e71:
      return Instruction::NOP;
    default:
      error("[m68k::CPU::decode] Unknown instruction [{:#x}]", (Word)ins);
      exit(1);
  }
}

}
