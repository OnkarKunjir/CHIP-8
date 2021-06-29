#include "cpu.hpp"
#include "utils/log.hpp"

int main(int argc, const char *argv[]) {
  if (argc != 2)
    Log::error("Usage", "CHIP-8 path_to_rom");
  CPU cpu;
  cpu.run(argv[1]);
  return 0;
}
