#pragma once
#include <vector>

#include "line.h"
#include "vt100.h"

namespace e {

class Window {
 public:
  virtual ~Window() = default;

  void Resize(int height, int width);
  void KeyPressed(char ch);

  Line& lines(int row) { return lines_[row]; }

  int height() const { return height_; }
  int width() const { return width_; }

  void ShowCursor() { is_cursor_visible_ = true; }
  void HideCursor() { is_cursor_visible_ = false; }
  bool IsCursorVisible() const { return is_cursor_visible_; }

  int cursor_row() const { return cursor_row_; }
  int cursor_col() const { return cursor_col_; }
  void MoveCursor(int row, int col) {
    cursor_row_ = row;
    cursor_row_ = col;
  }

 protected:
  virtual void OnResize() = 0;
  virtual void OnKeyPressed(char ch) = 0;

 private:
  std::vector<Line> lines_;

  bool is_cursor_visible_ = false;
  int cursor_row_ = 0;
  int cursor_col_ = 0;

  int height_ = 0;
  int width_ = 0;
};

class Terminal {
 public:
  explicit Terminal(Window& header, Window& main, Window& footer);
  ~Terminal();

  void WindowResized();

  void Redraw();

 private:
  Window& header_;
  Window& main_;
  Window& footer_;

  Window* focused_window_;

  int height_;
  int width_;

  Vt100Command command_;  // Buffer, allocated once to avoid reallocations.
};

}  // namespace  e