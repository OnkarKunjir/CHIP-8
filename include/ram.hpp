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
  /**
   *  @brief Functon makes each cell of ram null and stores default hex sprites
   * in interprter memory area.
   */
  void reset();

  /**
   *  @brief Access the memory contents.
   *  @param n Specify memory address.
   *  @return memory content at address n.
   */
  unsigned char &operator[](int n);
  /**
   *  @brief Access the memory contents.
   *  @param n Specify memory address.
   *  @return memory content at address n.
   */
  const unsigned char &operator[](int n) const;

  /**
   *  @brief Get address of defalut sprite.
   *  @pram sprite hex digit whoes address needs to be accessed.
   *  @param high_res query address of high resolution sprite if true.
   *  @return adderess of memory location containing sprite.
   */
  int sprite_location(unsigned char sprite, bool high_res = false) const;

  /**
   *  @brief Load the ROM contents into memory location starting at 0x200.
   *  @retrun address of first instruction in memory.
   */
  int load(const std::string &path);

  /**
   *  @brief Print contnets program space in RAM.
   */
  void dump() const;
};
