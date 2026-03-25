#include "line.h"

namespace e {

void Line::WindowResized(int window_width) {
  Check(0 < window_width);
  window_width_ = window_width;
  if (content_width() > window_width) {
    ResizeContent(window_width);  // Downsize but not upsize.
  }
}

void Line::ResizeContent(int width) {
  Check(0 <= width && width <= window_width_);
  cells_.resize(width);
}

void Line::ClearWithAttr(Attr attr) {
  for (Cell& cell : cells_) {
    cell.attr = attr;
  }
}

void Line::DrawTo(Vt100Command& command, int screen_row) {
  command.MoveTo(screen_row, 1);
  for (int j = 0; j < content_width(); j++) {
    const Cell& cell = cells(j);
    command.SetAttr(cell.attr);
    command.Print(cell.ch);
  }
  command.BgColor(background());
  command.TruncateLine();
}

}  // namespace e
