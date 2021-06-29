#pragma once

#define RAM_SIZE 4096

class Ram {
private:
  unsigned char _ram[RAM_SIZE];

public:
  Ram();
  void reset();

  unsigned char &operator[](int n);
  const unsigned char &operator[](int n) const;

  int sprite_location(unsigned char sprite) const;
};
