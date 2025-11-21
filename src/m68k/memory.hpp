#pragma once

#include <bit>
#include <concepts>

#include "types.hpp"

namespace m68k {

struct Addressable {
  virtual void read(Address addr, DataMutable data) = 0; // Read from address to data
  virtual void write(Address addr, DataImmutable data) = 0; // Write to address from data

  // Implementation of the read function based on the integral types (Byte, Word, LongWord, etc.)
  template<std::integral T>
  [[nodiscard]]
  T read(Address addr) {
    T data;
    read(addr, DataMutable{reinterpret_cast<Byte*>(&data), sizeof(T)});

    // To little-endian
    return std::byteswap(data);
  }

  // Implementation of the write function based on the integral types (Byte, Word, LongWord, etc.)
  template<std::integral T>
  void write(Address addr, T data) {
    data = std::byteswap(data); // To big-endian

    return write(addr, DataImmutable{reinterpret_cast<const Byte*>(&data), sizeof(T)});
  }
};

class MemoryBlock : public Addressable {
public:
  MemoryBlock(const std::span<const Word> rom, size_t ramLength);
  ~MemoryBlock();

  void load(const std::span<const Word> rom);

  void read(Address addr, DataMutable data) override;
  void write(Address addr, DataImmutable data) override;

private:
  Byte* content = new Byte[0];
  size_t contentLength = 0;
};

}

