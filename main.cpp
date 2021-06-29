#include "cpu.hpp"
#include "display.hpp"
#include "ram.hpp"
#include "utils/log.hpp"

#include <chrono>
#include <string>

int main() {

  Display display;
  Ram ram;
  ram.load("roms/TETRIS");

  CPU cpu;
  cpu.dump();

  int x = 0, y = 0;
  for (int i = 0; i <= 0xF; i++) {
    display.draw_sprite(ram, ram.sprite_location(i), 5, x, y);
    x += 8;
    if (x == 64) {
      x = 0;
      y += 6;
    }
  }

  while (display.is_active()) {
    auto start = std::chrono::high_resolution_clock::now();
    display.poll_events();
    display.update();
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    // Log::message("time", std::to_string(diff.count() * 1e-9));
    break;
  }
  return 0;
}
