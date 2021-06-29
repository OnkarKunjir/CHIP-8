#include "display.hpp"
#include "engine2d.hpp"
#include "shapes/rect.hpp"

#include <iostream>

Display::Display() : Engine2d("CHIP-8", DISPLAY_WIDTH, DISPLAY_HEIGHT) {
  cls();
}

void Display::cls() {
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      _grid[i][j] = false;
    }
  }
}

bool Display::draw_sprite(const std::vector<unsigned char> &sprite, int x,
                          int y) {

  bool collide = false;
  int row = y;
  int col = x;

  for (int i = 0; i < sprite.size(); i++) {
    col = x;
    for (int j = 0; j < 8; j++) {

      if (_grid[row % GRID_ROWS][col % GRID_COLS]) {
        _grid[row % GRID_ROWS][col % GRID_COLS] ^= (sprite[i] >> (7 - j)) & 1;
        collide = collide | !_grid[row % GRID_ROWS][col % GRID_COLS];
      } else {
        _grid[row % GRID_ROWS][col % GRID_COLS] ^= (sprite[i] >> (7 - j)) & 1;
      }

      col++;
    }
    row++;
  }
  return collide;
}

bool Display::draw_sprite(const Ram &ram, int start, int n, int x, int y) {
  bool collide = false;
  int row = y;
  int col = x;

  for (int i = start; i < start + n; i++) {
    col = x;
    for (int j = 0; j < 8; j++) {
      if (_grid[row % GRID_ROWS][col % GRID_COLS]) {
        _grid[row % GRID_ROWS][col % GRID_COLS] ^= (ram[i] >> (7 - j)) & 1;
        collide = collide | !_grid[row % GRID_ROWS][col % GRID_COLS];
      } else {
        _grid[row % GRID_ROWS][col % GRID_COLS] ^= (ram[i] >> (7 - j)) & 1;
      }
      col++;
    }
    row++;
  }
  return collide;
}

void Display::update() {
  std::vector<Rect> rect;

  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      if (_grid[i][j])
        rect.push_back({j * 10.0f, i * 10.0f, 10.0f, 10.0f});
    }
  }
  Engine2d::draw(rect);
  Engine2d::update();
}

bool Display::press(int key) const { return _window.press(keys[key]); }

bool Display::release(int key) const { return _window.release(keys[key]); }

double Display::get_time() const { return glfwGetTime(); }
