#include "terminal.h"

#include <sys/ioctl.h>
#include <unistd.h>

#include "line.h"
#include "util.h"
#include "vt100.h"

namespace e {

void Window::Resize(int height, int width) {
  height_ = height;
  width_ = width;
  lines_.resize(height);
  for (int i = 0; i < height; i++) {
    lines(i).WindowResized(width);
  }
  OnResize();
}

void Window::KeyPressed(char ch) { OnKeyPressed(ch); }

Terminal::Terminal(Window& header, Window& main, Window& footer)
    : header_(header), main_(main), footer_(footer), focused_window_(&main) {
  WindowResized();
}

Terminal::~Terminal() {}

void Terminal::WindowResized() {
  winsize ws;  // NOLINT(*-pro-type-member-init)
  CheckSyscall(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws));
  Check(ws.ws_col > 0 && ws.ws_row > 0, "Zero-size terminal window.");
  height_ = ws.ws_row;
  width_ = ws.ws_col;

  footer_.Resize(1, width_);
  main_.Resize(height_ - 2, width_);
  header_.Resize(1, width_);

  // Freeze top and bottom rows (we assume header and footer are always set).
  // Vt100Command().SetScrollRegion(2, height_ - 1).Send();

  Redraw();
}

void Terminal::Redraw() {
  command_.StartSyncUpdate();
  command_.HideCursor();
  command_.ClearScrollbackBuffer();
  int screen_row = 1;
  for (Window* window : {&header_, &main_, &footer_}) {
    for (int i = 0; i < window->height(); i++) {
      window->lines(i).DrawTo(command_, screen_row);
      screen_row++;
    }
  }
  command_.MoveTo(focused_window_->cursor_col(), focused_window_->cursor_row());
  if (focused_window_->IsCursorVisible()) {
    command_.ShowCursor();
  }
  command_.EndSyncUpdate();
  command_.Send();
}

}  // namespace e