#pragma once
#include <vector>

#include "util.h"
#include "vt100.h"

namespace e {

struct Cell {
  char ch;
  Attr attr;
};

class Line {
 public:
  int content_width() const { return cells_.size(); }

  void WindowResized(int window_width);

  void ResizeContent(int width);

  void ClearWithAttr(Attr attr);

  Cell& cells(int i) {
    Check(0 <= i && i < cells_.size());
    return cells_[i];
  }

  void SetBackground(Color color) { background_ = color; }
  Color background() const { return background_; }

  void DrawTo(Vt100Command& command, int screen_row);

 private:
  Color background_ = kBlack;
  int window_width_ = 0;
  std::vector<Cell> cells_;
};

}  // namespace e
