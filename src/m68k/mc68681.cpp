#include "mc68681.hpp"
#include "utils/utils.hpp"
#include <cstdint>
#include <print>

// Register Addresses
#define REG_SRA_RD (Address)0x03 // Ch A Status Register
#define REG_TBA_WR (Address)0x07 // Ch A Byte to Transmit
#define REG_RBA_RD (Address)0x07 // Ch A Received Byte
#define REG_ISR_RD (Address)0x0B // Interrupt Status Register

// Status Register Bits (SRA/SRB)
#define SR_TX_EMPTY (u8)0x08
#define SR_TX_READY (u8)0x04
#define SR_RX_FULL  (u8)0x02
#define SR_RX_READY (u8)0x01

// Interrupt Status/MAsk Bits(ISR/IVR)
#define ISR_TIMER_CHANGE (u8)0x08

namespace m68k {

void MC68681::read(Address addr, DataMutable data) {
  switch (addr) {
    case REG_SRA_RD:
      data[0] = SR_TX_READY | SR_TX_EMPTY;
      break;
    case REG_RBA_RD:
      data[0] = port_a_input;
      port_a_status &= !SR_RX_READY;
      break;
    case REG_ISR_RD:
      data[0] = int_status;
      break;
  }

  dprint("[m68k::MC68681::read] read from [{:#x}] of [{:#x}]", addr, data[0]);
}

void MC68681::write(Address addr, DataImmutable data) {
  dprint("[m68k::MC68681::read] writting [{:#x}] to [{:#x}]", data[0], addr);

  switch (addr) {
    case REG_TBA_WR:
      std::println("[m68k::MC68681::write] [{}]", data[0]);
      break;
  }
}

ClockElapsed MC68681::step(Clock clock) {
  if (is_timing) {
    timer = (UINT16_MAX + timer - 1) % UINT16_MAX;

    if (timer == 0) {
      int_status |= ISR_TIMER_CHANGE;
    }
  }

  // "Delay for the number of nanoseconds of our 3.6864 MHz clock"
  return 1e9 / 3'686'400;

}

consteval size_t MC68681::len() const {
  return 0x10;
}


}
