#pragma once

#define GP_REG 16     // number of general purpose registers.
#define STACK_SIZE 16 // stack size.

#include "display.hpp"
#include "ram.hpp"

#include <string>

class CPU {
private:
  char _V[GP_REG];            // 8-bit 16 general purpose register.
  unsigned char _delay_timer; // 8-bit delay timer.
  unsigned char _sound_timer; // 8-bit sound timer.
  char _SP;                   // 8-bit stack pointer.

  unsigned short int _I;                 // 16-bit register to hold address.
  unsigned short int _PC;                // 16-bit program counter.
  unsigned short int _stack[STACK_SIZE]; // 16-bit stack.

  Ram _ram;
  Display _display;

  bool _halt;

public:
  CPU();

  void dump() const;
  void reset();

  void run(const std::string &path);

private:
  unsigned short int fetch();
  void exec();
};
