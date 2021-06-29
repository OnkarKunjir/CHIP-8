#pragma once

#define RAM_SIZE 4096 // ram size in bytes.
#define OFFSET 0x200  // program section start offset.

#include <string>

class Ram {
private:
  unsigned char _ram[RAM_SIZE];
  int _rom_size;

public:
  Ram();
  void reset();

  unsigned char &operator[](int n);
  const unsigned char &operator[](int n) const;

  int sprite_location(unsigned char sprite) const;

  int load(const std::string &path);

  void dump() const;
};
