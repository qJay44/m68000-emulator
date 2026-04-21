#include "instructions.hpp"

#include <cassert>
#include <cstdlib>
#include <print>
#include <utility>

#include "target.hpp"
#include "utils/utils.hpp"

namespace m68k {

namespace {

Context* ctx = nullptr;
Word word;

void read() {
  assert(ctx);

  word = ctx->memory.read<Word>(ctx->state.pc);
  if (!word)
    error("[m68k::Instruction::read] No instruction [{:#x}] at [{:#x}]", word, ctx->state.pc);

  ctx->state.pc += 2;
}

/**
 * @param[in] at The bit index of the 16-bit word, should be in range 1-16 inclusively
 * @return The bit value
*/
bool bitAt(u8 at) {
  assert(at > 0);
  assert(at < 17);

  return word >> at & 0x1;
};

/**
 * @param[in] from The start bit index, should be in range 1-16 inclusively
 * @param[in] to   The end bit index, should be in range 1-16 inclusively
 * @return The value stored within the range (bits)
*/
u16 bitRange(u8 from, u8 to) {
  assert(from > 0);
  assert(to < 17);
  assert(from <= to);

  u16 shifted = word >> (16 - to);
  return shifted & (UINT16_MAX >> (16 - (to - from + 1)));
};

// ORI to CCR, ORI, ..., MOVEP
bool isPattern1() {
  return word >> 11 & 1;
}

// BTST, BCHG, BCLR, BSET, MOVEP
bool isPattern1_1() {
  return word >> 8 & 1;
}

// MOVEP
bool isPattern1_1_1() {
  return (word >> 3) == 1;
}

// MOVEA, MOVE
bool isPattern2() {
  return word & 0x3000;
}

// Purple
Instruction::Size parseSize1(Byte byte) {
  return Instruction::Size((byte >> 1) + (1 - bool(byte)));
}

// Darker Purple
Instruction::Size parseSize2(Byte byte) {
  return Instruction::Size(byte * 2 + 2);
}

// Darkest Purple
Instruction::Size parseSize3(Byte byte) {
  switch (byte) {
    case 1:
      return Instruction::Size::Byte;
    case 3:
      return Instruction::Size::Word;
    case 2:
      return Instruction::Size::Long;
    default:
      error("[m68k::Instruction::parseSize3] Unexpected value [{}]", byte);
      exit(1);
  }
}

Target parseTargetWithSize(Instruction::Size size) {
  u16 mode = bitRange(11, 13);
  u16 xn = bitRange(14, 16);
  Target target{};

  switch (mode) {
    case 0:
      target.kind = Target::Kind::DataRegister;
      target.index = xn;
      break;
    case 1:
      target.kind = Target::Kind::AddressRegister;
      target.index = xn;
      break;
    case 2:
      target.kind = Target::Kind::Address;
      target.index = xn;
      break;
    case 3:
      target.kind = Target::Kind::AddressPostincrement;
      target.index = xn;
      target.size = (Byte)size;
      break;
    case 4:
      target.kind = Target::Kind::AddressPredecrement;
      target.index = xn;
      target.size = (Byte)size;
      break;
    case 5:
      read();
      target.kind = Target::Kind::AddressDisplacement;
      target.index = xn;
      target.extWord0 = word;
      break;
    case 6:
      read();
      target.kind = Target::Kind::AddressIndex;
      target.index = xn;
      target.extWord0 = word;
      break;
    case 7: {
      switch (xn) {
        case 2:
          read();
          target.kind = Target::Kind::ProgramCounterDisplacement;
          target.extWord0 = word;
          break;
        case 3:
          read();
          target.kind = Target::Kind::ProgramCounterIndex;
          target.extWord0 = word;
          break;
        case 0:
          read();
          target.kind = Target::Kind::AbsoluteShort;
          target.extWord0 = word;
          break;
        case 1: {
          read();
          target.kind = Target::Kind::AbsoluteLong;
          target.extWord0 = word;
          read();
          target.extWord1 = word;
          break;
        }
        case 4: {
          auto& pc = ctx->state.pc;
          target.kind = Target::Kind::Immeadiate;
          target.address = size == Instruction::Size::Byte ? pc + 1 : pc;
          pc += size == Instruction::Size::Long ? 4 : 2;
          break;
        }
        default:
          error("[m68k::Instruction::parseTargetWithSize] Unexpected addressing index [{}]", xn);
      }
      break;
    }
    default:
      error("[m68k::Instruction::parseTargetWithSize] Unexpected addressing mode [{}]", mode);
  }

  return target;
}

} // namespace

Instruction Instruction::decode(Context& context) {
  ctx = &context;
  read();

  Instruction ins{};

  if (isPattern1()) {
    if (isPattern1_1()) {
      if (isPattern1_1_1()) {
        ins.kind = Kind::MOVEP;
        ins.size = parseSize2(bitAt(6));
        ins.src.kind = Target::Kind::DataRegister;
        ins.dst.kind = Target::Kind::AddressDisplacement;

        // Dark red direction
        if (!bitAt(7)) {
          std::swap(ins.src, ins.dst);
        }
      } else {
        const u8 id = bitRange(9, 10);
        switch (id) {
          case 0: {
            auto src = Target{};
            src.kind = Target::Kind::DataRegister;
            src.index = bitRange(5, 8);

            ins.kind = Kind::BTST;
            ins.src = src;
            ins.dst = parseTargetWithSize(Size::Byte);
            break;
          }
          case 1: {
            auto src = Target{};
            src.kind = Target::Kind::DataRegister;
            src.index = bitRange(5, 8);

            ins.kind = Kind::BCHG;
            ins.src = src;
            ins.dst = parseTargetWithSize(Size::Byte);
            break;
          }
          case 2: {
            auto src = Target{};
            src.kind = Target::Kind::DataRegister;
            src.index = bitRange(5, 8);

            ins.kind = Kind::BCLR;
            ins.src = src;
            ins.dst = parseTargetWithSize(Size::Byte);
            break;
          }
          case 3: {
            auto src = Target{};
            src.kind = Target::Kind::DataRegister;
            src.index = bitRange(5, 8);

            ins.kind = Kind::BSET;
            ins.src = src;
            ins.dst = parseTargetWithSize(Size::Byte);
            break;
          }
          default:
            error("[m68k::Instruction::decode] Unexpected value [{}] for [{}] instructions", id, "BTST, BCHG, BCLR, BSET");
        }

      }
    }
  }

  else if (isPattern2()) {
    std::println("test");

  }

  std::println("test end");
  exit(0);

  // switch (word) {
  //   case 0x4e71:
  //     ins.kind = Kind::NOP;
  //     break;
  //   default:
  //     error("[m68k::CPU::decode] Unknown instruction [{:#x}]", (Word)word);
  // }

  return ins;
}

void Instruction::execute() {

}

}

