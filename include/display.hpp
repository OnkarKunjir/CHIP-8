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

  /**
   *  @brief Clear all the pixels form display.
   */
  void cls();

  /**
   *  @brief Draw sprite from vector.
   *  @param sprite Vector containing sprite.
   *  @param x top left x-coordinate of sprite on screen.
   *  @pram y top left y-coordinate of sprite on screen.
   *  @return true if any pixel is erased due to sprite.
   */
  bool draw_sprite(const std::vector<unsigned char> &sprite, int x, int y);
  /**
   *  @brief Draw sprite from RAM content.
   *  @param ram Specify instance of Ram.
   *  @param start Statring address of sprite in ram.
   *  @param n Size of sprite to be drawn in bytes.
   *  @param x top left x-coordinate of sprite on screen.
   *  @pram y top left y-coordinate of sprite on screen.
   *  @return true if any pixel is erased due to sprite.
   */
  bool draw_sprite(const Ram &ram, int start, int n, int x, int y);

  /**
   * @brief Update the display.
   */
  void update();

  /**
   *  @brief Function to check if key from keys is pressed.
   *  @param key Index of value form array keys.
   *  @return true if key is pressed.
   */
  bool press(int key) const;
  /**
   *  @brief Function to check if key from keys is not pressed.
   *  @param key Index of value form array keys.
   *  @return true if key is not pressed.
   */
  bool release(int key) const;

  /**
   *  @return Time elasped since creating of glfw context.
   */
  double get_time() const;

  /**
   *  @brief Wait till any key form keys is pressed.
   *  @return index of key which was pressed.
   */
  unsigned char wait_for_key() const;
};
