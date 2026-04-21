#pragma once

#include "Context.hpp"
#include "target.hpp"
#include "types.hpp"
#include "memory.hpp"
#include "state.hpp"

namespace m68k {

struct Instruction {
  enum class Kind : Byte {
    ABCD,
    ADD,
    ADDA,
    ADDI,
    ADDQ,
    ADDX,
    AND,
    ANDI,
    ANDITOCCR,
    ANDITOSR,
    ASL,
    ASR,
    BCC,
    BCHG,
    BCLR,
    BSET,
    BSR,
    BTST,
    CHK,
    CLR,
    CMP,
    CMPA,
    CMPI,
    CMPM,
    DBCC,
    DIVS,
    DIVU,
    EOR,
    EORI,
    EORITOCCR,
    EORITOSR,
    EXG,
    EXT,
    JMP,
    JSR,
    LEA,
    LINK,
    LSL,
    LSR,
    MOVEFROMSR,
    MOVEFROMUSP,
    MOVE,
    MOVETOCCR,
    MOVETOSR,
    MOVETOUSP,
    MOVEA,
    MOVEM,
    MOVEP,
    MOVEQ,
    MULS,
    MULU,
    NBCD,
    NEG,
    NEGX,
    NOP,
    NOT,
    OR,
    ORI,
    ORITOCCR,
    ORITOSR,
    PEA,
    RESET,
    ROL,
    ROR,
    ROXL,
    ROXR,
    RTE,
    RTR,
    RTS,
    SBCD,
    SCC,
    SUB,
    SUBA,
    SUBI,
    SUBQ,
    SUBX,
    SWAP,
    TAS,
    TRAP,
    TRAPV,
    TST,
    UNLINK,
  };

  enum class Size : Byte {
    Byte = 1,
    Word = 2,
    Long = 4,
  };

  enum class Condition : Byte {
    True,           // T
    False,          // F
    Higher,         // HI
    LowerOrSame,    // LS
    CarryClear,     // CC
    CarrySet,       // CS
    NotEqual,       // NE
    Equal,          // EQ
    OverflowClear,  // VC
    OverflowSet,    // VS
    Plus,           // PL
    Minus,          // MI
    GreaterOrEqual, // GE
    LessThan,       // LT
    GreaterThan,    // GT
    LessOrEqual,    // LE
  };

  Kind kind;
  Size size;
  Condition cond;
  Target src;
  Target dst;
  LongWord data;

  [[nodiscard]]
  static Instruction decode(Context& context);

  void execute();
};

}

