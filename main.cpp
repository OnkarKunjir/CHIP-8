#include "cpu.hpp"
#include "utils/log.hpp"

#include <string>

int main(int argc, const char *argv[]) {
  if (argc < 2)
    Log::error("Usage", "CHIP-8 [--force] path_to_rom");

  CPU cpu;
  if (argc == 2) {
    cpu.run(argv[1]);
  } else if (argc == 3 && std::string(argv[1]) == "--force") {
    cpu.force();
    cpu.run(argv[2]);
  } else
    Log::error("Usage", "CHIP-8 [--force] path_to_rom");

  return 0;
}
