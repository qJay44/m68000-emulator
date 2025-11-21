#include <print>
#include <unistd.h>

#include "m68k/cpu.hpp"
#include "m68k/memory.hpp"

int main() {
  std::println("_Start");

  constexpr u16 rom[10] = {
    0x0010, 0x0000,     // Initial stack address is at 0x00100000
    0x0000, 0x0008,     // Initial PC address is at 0x8, which is the word
                        //  that follows this

    0x4e71,             // 4 NOP instructions
    0x4e71,
    0x4e71,
    0x4e71,

    0x4e72, 0x2700      // The STOP #0x2700 instruction, which would normally
                        //  stop the CPU but it's unsupported, so it will
                        //  cause an error
  };

  m68k::CPU cpu;
  m68k::MemoryBlock memory((std::span<const u16>(rom)), 4096);

  for (int i = 0; i < 10; i++) {
    cpu.step(memory);
  }

  std::println("_Exit");

  return 0;
}

