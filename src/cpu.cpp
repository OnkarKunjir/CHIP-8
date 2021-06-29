#include "utils/log.hpp"
#define __FILENAME__ "CPU"

#include "cpu.hpp"

#include <cstdio>

CPU::CPU() { reset(); }

void CPU::reset() {
  _halt = false;
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

void CPU::run(const std::string &path) {
  reset();
  _PC = _ram.load(path);
  while (!_halt) {
    // do processing.
    exec();
    _halt = true;
  }
}

// private functions.
unsigned short int CPU::fetch() {
  unsigned short int instruction = 0;
  instruction = _ram[_PC++];
  instruction = (instruction << 8) | _ram[_PC++];
  return instruction;
}

void CPU::exec() {
  unsigned short int instruction = fetch();

  // decode instructions
  unsigned char group = (instruction & 0xF000) >> 12;
  unsigned char x = (instruction & 0x0F00) >> 8;
  unsigned char y = (instruction & 0x00F0) >> 4;
  unsigned char kk = instruction & 0x00FF;
  unsigned char n = instruction & 0x000F;
  unsigned short int nnn = instruction & 0x0FFF;

  switch (group) {

  default:
    printf("%04X\n", instruction);
    Log::error(__FILENAME__, "Invalid instruction");
    break;
  }
}
