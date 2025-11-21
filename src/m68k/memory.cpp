#include "memory.hpp"

#include <bit>
#include <cassert>
#include <cstdlib>
#include <cstring>

namespace m68k {

MemoryBlock::MemoryBlock(const std::span<const Word> rom, size_t ramLength) {
  contentLength = ramLength;
  load(rom);
}

MemoryBlock::~MemoryBlock() {
  delete[] content;
}

void MemoryBlock::load(const std::span<const Word> rom) {
  delete[] content;
  content = new Byte[contentLength];

  assert(rom.size() <= contentLength);

  for (size_t i = 0; i < rom.size(); i++) {
    Word word = rom[i];
    word = std::byteswap(word); // To big-endian
    memcpy(content + i * 2, &word, 2);
  }
}

void MemoryBlock::read(Address addr, DataMutable data) {
  assert(content);
  assert(addr < contentLength);
  memcpy(data.data(), content + addr, data.size_bytes());
}

void MemoryBlock::write(Address addr, DataImmutable data) {
  assert(content);
  assert(addr < contentLength);
  memcpy(content + addr, data.data(), data.size_bytes());
}

}

