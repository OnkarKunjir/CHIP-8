#include "display.hpp"

Display::Display() : Engine2d("CHIP-8", DISPLAY_WIDTH, DISPLAY_HEIGHT) {
  cls();
  _high_res = false;
  _pixel_updated = true;
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
  // FIXME: update this function.
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

  _pixel_updated = true;
  if (n == 0 && _high_res)
    return draw_high_res_sprite(ram, start, x, y);

  int grid_rows = _high_res ? GRID_ROWS : LOW_RES_GRID_ROWS;
  int grid_cols = _high_res ? GRID_COLS : LOW_RES_GRID_COLS;

  bool collide = false;
  int row = y % grid_rows;
  int col = x % grid_cols;

  for (int i = start; i < (start + n); i++) {
    col = x % grid_cols;
    for (int j = 0; j < 8; j++) {
      if ((ram[i] & (0x80 >> j)) != 0) {
        if (row < GRID_ROWS && col < GRID_COLS) {
          if (_grid[row][col])
            collide = true;
          _grid[row][col] ^= 1;
        }
      }
      col = (col + 1) % grid_cols;
    }
    row = (row + 1) % grid_rows;
  }
  return collide;
}

void Display::update() {
  // scale the pixel according to dispaly mode.
  if (_pixel_updated) {
    std::vector<Rect> rect;
    float pixel_size = _high_res ? 10.0f : 20.0f;

    for (int i = 0; i < GRID_ROWS; i++) {
      for (int j = 0; j < GRID_COLS; j++) {
        if (_grid[i][j]) {
          rect.push_back(
              {j * pixel_size, i * pixel_size, pixel_size, pixel_size});
        }
      }
    }
    Engine2d::draw(rect);
    _pixel_updated = false;
  } else {
    Engine2d::draw();
  }
  Engine2d::update();
}

bool Display::press(int key) const { return _window.press(keys[key]); }

bool Display::release(int key) const { return _window.release(keys[key]); }

double Display::get_time() const { return glfwGetTime(); }

unsigned char Display::wait_for_key() const {
  while (true) {
    for (int i = 0; i < 16; i++) {
      if (press(i))
        return i;
    }
    poll_events();
  }
}

void Display::high_res(bool enable) { _high_res = enable; }

void Display::scroll_down(unsigned char n) {
  _pixel_updated = true;
  for (int i = GRID_ROWS - 1; i > 0; i--) {
    for (int j = 0; j < GRID_COLS; j++) {
      _grid[i][j] = _grid[i - 1][j];
    }
  }
  for (int i = 0; i < GRID_COLS; i++)
    _grid[0][i] = false;
}

void Display::scroll_left() {
  _pixel_updated = true;
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS - 4; j++) {
      _grid[i][j] = _grid[i][j + 4];
    }
  }
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = GRID_COLS - 4; j < GRID_COLS; j++) {
      _grid[i][j] = false;
    }
  }
}

void Display::scroll_right() {
  _pixel_updated = true;
  // NOTE: Function is not tested yet.
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = GRID_COLS - 4; j > 4; j--) {
      _grid[i][j] = _grid[i][j - 4];
    }
  }
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < 4; j++) {
      _grid[i][j] = false;
    }
  }
}

// private functions
bool Display::draw_high_res_sprite(const Ram &ram, int start, int x, int y) {

  int grid_rows = GRID_ROWS;
  int grid_cols = GRID_COLS;

  bool collide = false;
  int row = y % grid_rows;
  int col = x % grid_cols;

  unsigned short int bytes = 0;
  int index = start;

  for (int i = 0; i < 16; i++) {
    int col = x % grid_cols;
    bytes = ram[index++];
    bytes = (bytes << 8) | ram[index++];
    for (int j = 0; j < 16; j++) {

      if ((bytes & (0x8000 >> j)) != 0) {
        if (row < GRID_ROWS && col < GRID_COLS) {
          if (_grid[row][col])
            collide = true;
          _grid[row][col] ^= 1;
        }
      }
      col = (col + 1) % grid_cols;
    }
    row = (row + 1) % grid_rows;
  }

  return collide;
}
