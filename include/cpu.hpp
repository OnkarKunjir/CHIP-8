#pragma once

#define GP_REG 16     // number of general purpose registers.
#define STACK_SIZE 16 // stack size.

#include "display.hpp"
#include "ram.hpp"

#include <stack>
#include <string>

class CPU {
private:
  char _V[GP_REG];            // 8-bit 16 general purpose register.
  unsigned char _delay_timer; // 8-bit delay timer.
  unsigned char _sound_timer; // 8-bit sound timer.

  unsigned short int _I;                 // 16-bit register to hold address.
  unsigned short int _PC;                // 16-bit program counter.
  std::stack<unsigned short int> _stack; // stack used for function calls.

  Ram _ram;
  Display _display;

  bool _halt;

public:
  CPU();

  /**
   *  @brief Function to print contents of registers.
   */
  void dump() const;
  /**
   *  @brief Function to reset CPU state.
   */
  void reset();

  /**
   *  @brief Executes the rom.
   *  @param path Specify the path of ram.
   */
  void run(const std::string &path);

private:
  /**
   *  @brief Fetch 2 byte instruction from RAM and update program counter.
   *  @return 2 byte instruction.
   */
  unsigned short int fetch();
  /**
   *  @brief Decode and execute instruction pointed by program counter.
   */
  void exec();
};
