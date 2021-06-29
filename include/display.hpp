#pragma once

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 320

#define GRID_ROWS 32
#define GRID_COLS 64

#include "ram.hpp"

#include <engine2d.hpp>
#include <vector>

class Display : public Engine2d {
private:
  bool _grid[GRID_ROWS][GRID_COLS];

public:
  Display();

  void cls();

  bool draw_sprite(const std::vector<unsigned char> &sprite, int x, int y);
  bool draw_sprite(const Ram &ram, int start, int n, int x, int y);

  void update();
};
