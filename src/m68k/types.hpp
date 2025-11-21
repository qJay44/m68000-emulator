#pragma once

#include <bit>
#include <span>

#include "utils/types.hpp"

namespace m68k {

using Byte = u8;
using Word = u16;
using Address = u64;
using LongWord = u32;
using QuadWord = u64;

using DataMutable = std::span<Byte>;

struct DataImmutable : public std::span<const Byte> {
  using Base = std::span<const Byte>;
  using Base::Base; // Using std::span's constructors

  // Get the data as T (Byte, Word, LongWord, etc.)
  template<std::integral T>
  T as() const {
    return std::byteswap(*reinterpret_cast<const T*>(data()));
  }
};

}

