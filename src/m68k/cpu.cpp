#include "cpu.hpp"

#include <print>

#include "Context.hpp"
#include "instructions.hpp"
#include "types.hpp"
#include "utils/utils.hpp"

namespace m68k {

void CPU::init(Addressable& memory) {
  state.ssp = memory.read<LongWord>(0);
  state.pc = memory.read<LongWord>(4);
  state.status = Status::Running;
}

void CPU::step(Addressable& memory) {
  switch (state.status) {
    case Status::Init:
      init(memory);
      break;
    case Status::Running: {
      LongWord addr = state.pc;
      Context ctx(state, memory);
      Instruction ins = Instruction::decode(ctx);
      std::println("pc: {:#x}, instruction: {}", addr, (Byte)ins.kind);
      ins.execute();
      break;
    }
    case Status::Stopped:
      warning("CPU has Stopped");
      break;
  }
}

}

