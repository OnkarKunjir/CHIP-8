#include "cpu.hpp"

#include <cstdio>

CPU::CPU() { reset(); }

void CPU::reset() {
  _delay_timer = 0;
  _sound_timer = 0;
  _SP = -1;
  _I = 0;
  _PC = 0;
  for (int i = 0; i < STACK_SIZE; i++)
    _stack[i] = 0;
  for (int i = 0; i < GP_REG; i++)
    _V[i] = 0;
}

void CPU::dump() const {
  for (int i = 0; i < GP_REG; i++)
    printf("%01X  |", i);
  printf("\n");
  for (int i = 0; i < GP_REG; i++)
    printf("%02X |", (unsigned char)_V[i]);
  printf("\n");

  printf("PC : 0x%03X\t SP : %02X\n", _PC, (unsigned char)_SP);
}

void CPU::run(const std::string &path) {}
