#define __FILENAME__ "CPU"

#include "cpu.hpp"
#include "utils/log.hpp"

#include <cstdio>

CPU::CPU() { reset(); }

void CPU::reset() {
  _halt = false;
  _delay_timer = 0;
  _sound_timer = 0;
  _I = 0;
  _PC = 0;
  for (int i = 0; i < GP_REG; i++)
    _V[i] = 0;

  for (int i = 0; i < RPL_REG; i++)
    _RPL[i] = 0;
}

void CPU::dump() const {
  for (int i = 0; i < GP_REG; i++)
    printf("%01X  |", i);
  printf("\n");
  for (int i = 0; i < GP_REG; i++)
    printf("%02X |", (unsigned char)_V[i]);
  printf("\n");
  printf("PC : 0x%03X\t SP : %d\t I : %03X\n", _PC, (int)_stack.size(), _I);
}

void CPU::run(const std::string &path) {
  reset();
  _PC = _ram.load(path);

  double delay_last_time = 0, delay_current_time = 0;
  double fps_last_time = 0, fps_current_time = 0;

  while (!_halt && _display.is_active()) {
    fps_current_time = _display.get_time();
    if (fps_current_time - fps_last_time > 1.0 / MAX_FPS) {
      fps_last_time = fps_current_time;
      exec();
      _display.update();
      _display.poll_events();
    }

    delay_current_time = _display.get_time();
    if (delay_current_time - delay_last_time > 1.0 / CLOCK_FREQ) {
      delay_last_time = delay_current_time;
      // do processing.

      if (_delay_timer > 0)
        _delay_timer--;
      if (_sound_timer > 0)
        _sound_timer--;
    }
  }
}

void CPU::force() { _display.high_res(true); }

// private functions.
unsigned short int CPU::fetch() {
  unsigned short int instruction = 0;
  instruction = _ram[_PC++];
  instruction = (instruction << 8) | _ram[_PC++];
  return instruction;
}

void CPU::exec() {
  unsigned short int instruction = fetch();
  // printf("Instruction : %04X\n\n", instruction);

  // decode instructions
  unsigned char group = (instruction & 0xF000) >> 12;
  unsigned char x = (instruction & 0x0F00) >> 8;
  unsigned char y = (instruction & 0x00F0) >> 4;
  unsigned char kk = instruction & 0x00FF;
  unsigned char n = instruction & 0x000F;
  unsigned short int nnn = instruction & 0x0FFF;

  switch (group) {

  case 0x0:
    if (instruction == 0x00E0) {
      // CLS
      _display.cls();
    } else if (instruction == 0x00EE) {
      // RET
      if (_stack.empty())
        Log::error(__FILENAME__, "Stack underlfow");
      _PC = _stack.top();
      _stack.pop();
    } else if (instruction == 0x00FD) {
      // EXIT
      _halt = true;
    } else if (instruction == 0x00FB) {
      // Scroll right.
      _display.scroll_right();
    } else if (instruction == 0x00FC) {
      // Scroll left.
      _display.scroll_left();
    } else if (instruction == 0x00FE) {
      // Disable extended screen mode.
      _display.high_res(false);
    } else if (instruction == 0x00FF) {
      // Enable extended screen mode for full-screen graphics.
      _display.high_res(true);
    } else if (y == 0xC) {
      // Scroll down n pixels.
      _display.scroll_down(n);
    } else {
      printf("%04X\n", instruction);
      Log::error(__FILENAME__, "Invalid instruction");
    }
    break;

  case 0x1:
    // JP addr
    _PC = nnn;
    break;

  case 0x2:
    // CALL nnn
    if (_stack.size() == STACK_SIZE)
      Log::error(__FILENAME__, "Stack overflow");
    _stack.push(_PC);
    _PC = nnn;
    break;

  case 0x3:
    // SE Vx, byte
    if (_V[x] == kk)
      _PC += 2;
    break;

  case 0x4:
    // SNE Vx, byte
    if (_V[x] != kk)
      _PC += 2;
    break;

  case 0x5:
    // SE Vx, Vy
    if (_V[x] == _V[y])
      _PC += 2;
    break;

  case 0x6:
    // LD Vx, byte
    _V[x] = kk;
    break;

  case 0x7:
    // ADD Vx, byte
    _V[x] += kk;
    break;

  case 0x8:
    switch (n) {

    case 0x0:
      // LD Vx, Vy
      _V[x] = _V[y];
      break;

    case 0x1:
      // OR Vx, Vy
      _V[x] = _V[x] | _V[y];
      break;

    case 0x2:
      // AND Vx, Vy
      _V[x] = _V[x] & _V[y];
      break;

    case 0x3:
      // XOR Vx, Vy
      _V[x] = _V[x] ^ _V[y];
      break;

    case 0x4:
      // ADD Vx, Vy
      _V[0xF] = (_V[x] + _V[y]) > 0xFF;
      _V[x] = _V[x] + _V[y];
      break;

    case 0x5:
      // SUB Vx, Vy
      _V[0xF] = _V[x] > _V[y];
      _V[x] = _V[x] - _V[y];
      break;

    case 0x6:
      // SHR Vx {, Vy}
      _V[0xF] = _V[x] & 1;
      _V[x] = _V[x] / 2;
      break;

    case 0x7:
      // SUBN Vx, Vy
      _V[0xF] = _V[y] > _V[x];
      _V[x] = _V[y] - _V[x];
      break;

    case 0xE:
      // SHL Vx {, Vy}
      _V[0xF] = (_V[x] & 0x80) >> 7;
      _V[x] = _V[x] * 2;
      break;

    default:
      printf("%04X\n", instruction);
      Log::error(__FILENAME__, "Invalid instruction");
      break;
    }
    break;

  case 0x9:
    // SNE Vx, Vy
    if (_V[x] != _V[y])
      _PC += 2;
    break;

  case 0xA:
    // LD I, addr
    _I = nnn;
    break;

  case 0xB:
    // JP V0, addr
    _PC = _V[0] + nnn;
    break;

  case 0xC:
    // RND Vx, byte
    _V[x] = rand() & kk;
    break;

  case 0xD:
    // DRW Vx, Vy, nibble.
    _V[0xf] = _display.draw_sprite(_ram, _I, n, _V[x], _V[y]);
    break;

  case 0xE:
    switch (kk) {

    case 0x9E:
      // SKP Vx
      if (_display.press(_V[x]))
        _PC += 2;
      break;

    case 0xA1:
      // SKNP Vx
      if (_display.release(_V[x]))
        _PC += 2;
      break;

    default:
      printf("%04X\n", instruction);
      Log::error(__FILENAME__, "Invalid instruction");
      break;
    }
    break;

  case 0xF:
    switch (kk) {
    case 0x07:
      // LD Vx, DT
      _V[x] = _delay_timer;
      break;

    case 0x0A:
      // LD Vx, K
      Log::warn(__FILENAME__, "Waiting for key press");
      _V[x] = _display.wait_for_key();
      break;

    case 0x15:
      // LD DT, Vx
      _delay_timer = _V[x];
      break;

    case 0x18:
      // LD ST, Vx
      _sound_timer = _V[x];
      break;

    case 0x1E:
      // ADD I, Vx
      _I = _I + _V[x];
      break;

    case 0x29:
      // LD F, Vx
      _I = _ram.sprite_location(_V[x]);
      break;

    case 0x30:
      // LD F, Vx
      // Point I to 10-byte font sprite for digit VX (0..9)
      _I = _ram.sprite_location(_V[x], true);
      break;

    case 0x33:
      // LD B, Vx
      kk = _V[x];
      _ram[_I + 2] = kk % 10;
      kk = kk / 10;
      _ram[_I + 1] = kk % 10;
      kk = kk / 10;
      _ram[_I] = kk % 10;
      break;

    case 0x55:
      for (int i = 0; i <= x; i++) {
        _ram[_I + i] = _V[i];
      }
      break;

    case 0x65:
      for (int i = 0; i <= x; i++) {
        _V[i] = _ram[_I + i];
      }
      break;

    case 0x75:
      // Fx75
      // Store V0..VX in RPL user flags (X <= 7)
      if (x > 7)
        Log::error(__FILENAME__, "Invalid RPL register write");
      for (int i = 0; i <= x; i++)
        _RPL[i] = _V[i];
      break;

    case 0x85:
      // Fx85
      // Read V0..VX from RPL user flags (X <= 7)
      if (x > 7)
        Log::error(__FILENAME__, "Invalid RPL register read");
      for (int i = 0; i <= x; i++)
        _V[i] = _RPL[i];
      break;

    default:
      printf("%04X\n", instruction);
      Log::error(__FILENAME__, "Invalid instruction");
      break;
    }
    break;

  default:
    printf("%04X\n", instruction);
    Log::error(__FILENAME__, "Invalid instruction");
    break;
  }
}
