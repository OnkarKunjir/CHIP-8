#define __FILENAME__ "RAM"

#include "ram.hpp"
#include "utils/log.hpp"

#include <cstdio>
#include <fstream>

Ram::Ram() { reset(); }

void Ram::reset() {
  unsigned char default_sprite[] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10,
      0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10,
      0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0,
      0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0,
      0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0,
      0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80,
      0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80,
  };

  for (int i = 0; i < RAM_SIZE; i++)
    _ram[i] = 0;

  for (int i = 0; i < sizeof(default_sprite); i++)
    _ram[i] = default_sprite[i];
}

unsigned char &Ram::operator[](int n) {
  if (n < 0 || n > RAM_SIZE)
    Log::error(__FILENAME__, "Invalid address");
  return _ram[n];
}

const unsigned char &Ram::operator[](int n) const {
  if (n < 0 || n > RAM_SIZE)
    Log::error(__FILENAME__, "Invalid address");
  return _ram[n];
}

int Ram::sprite_location(unsigned char sprite) const { return sprite * 5; }

int Ram::load(const std::string &path) {
  std::fstream rom(path, std::ios::in);

  if (!rom.is_open())
    Log::error(__FILENAME__, "Failed to load rom: " + path);

  rom.seekg(std::ios::beg, std::ios::end);
  _rom_size = rom.tellg();
  rom.seekg(std::ios::beg, std::ios::beg);
  Log::warn("ROM SIZE", std::to_string(_rom_size));

  rom.read((char *)_ram + OFFSET, _rom_size);

  rom.close();
  return OFFSET;
}

void Ram::dump() const {
  for (int i = OFFSET; i < OFFSET + _rom_size - 1; i += 2) {
    printf("%03X | %02x %02x\n", i, _ram[i], _ram[i + 1]);
  }
}
