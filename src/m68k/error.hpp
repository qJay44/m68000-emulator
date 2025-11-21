#pragma once

#include <cstdlib>
#include <string>

#include "utils/utils.hpp"

namespace m68k {

class Error {
public:
  enum Kind : u32 {
    // no error
    Ok,

    UnalignedMemoryRead,
    UnalignedMemoryWrite,
    UnalignedProgramCounter,
    UnknownAddressingMode,
    UnknownOpcode,

    // permission error
    ProtectedRead,
    ProtectedWrite,

    // bus error
    UnmappedRead,
    UnmappedWrite,

    // invalid action
    InvalidRead,
    InvalidWrite,
  };

  Error() = default;
  Error(Kind kind, std::string what): kind_{kind}, what_{std::move(what)} {
    if (kind_ != Ok) {
      error("[m68k::Error] kind: [{}], what: [{}]", (u32)kind_, what_);
      exit(EXIT_FAILURE);
    }
  }

  Kind kind() const {
    return kind_;
  }
  const std::string& what() const {
    return what_;
  }

private:
  Kind kind_{Ok};
  std::string what_;
};

}

