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
  unsigned int keys[16] = {
      GLFW_KEY_X, // 0
      GLFW_KEY_1, // 1
      GLFW_KEY_2, // 2
      GLFW_KEY_3, // 3
      GLFW_KEY_Q, // 4
      GLFW_KEY_W, // 5
      GLFW_KEY_E, // 6
      GLFW_KEY_A, // 7
      GLFW_KEY_S, // 8
      GLFW_KEY_D, // 9
      GLFW_KEY_Z, // A
      GLFW_KEY_C, // B
      GLFW_KEY_4, // C
      GLFW_KEY_R, // D
      GLFW_KEY_F, // E
      GLFW_KEY_V, // F

  };

  Display();

  void cls();

  bool draw_sprite(const std::vector<unsigned char> &sprite, int x, int y);
  bool draw_sprite(const Ram &ram, int start, int n, int x, int y);

  void update();

  bool press(int key) const;
  bool release(int key) const;

  double get_time() const;

  unsigned char wait_for_key() const;
};
