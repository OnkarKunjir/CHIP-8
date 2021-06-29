#include "cpu.hpp"
#include "display.hpp"
#include "ram.hpp"
#include "utils/log.hpp"

#include <chrono>
#include <string>

int main() {
  CPU cpu;
  cpu.run("roms/TETRIS");
  return 0;
}
