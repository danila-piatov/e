#include "vt100.h"

#include <unistd.h>

#include <format>

#include "util.h"

namespace e {

Vt100Command& Vt100Command::Reserve(int capacity) {
  buf_.reserve(capacity);
  return *this;
}

Vt100Command& Vt100Command::AlternativeBufferOn() {
  return Print("\033[?1049h");
}

Vt100Command& Vt100Command::AlternativeBufferOff() {
  return Print("\033[?1049l");
}

Vt100Command& Vt100Command::StartSyncUpdate() { return Print("\033[?2026h"); }
Vt100Command& Vt100Command::EndSyncUpdate() { return Print("\033[?2026l"); }
Vt100Command& Vt100Command::HideCursor() { return Print("\033[?25l"); }
Vt100Command& Vt100Command::ShowCursor() { return Print("\033[?25h"); }
Vt100Command& Vt100Command::TruncateLine() { return Print("\033[K"); }
Vt100Command& Vt100Command::ClearScreen() { return Print("\033[2J"); }

Vt100Command& Vt100Command::ClearScrollbackBuffer() { return Print("\033[3J"); }

Vt100Command& Vt100Command::ResetStyle() { return Print("\033[m"); }

Vt100Command& Vt100Command::MoveTo(int row, int col) {
  return PrintFormat("\033[{};{}H", row, col);
}

Vt100Command& Vt100Command::SetScrollRegion(int top, int bottom) {
  return PrintFormat("\033[{};{}r", top, bottom);
}

Vt100Command& Vt100Command::TextColor(Color color) {
  return PrintFormat("\033[38;5;{}m", color);
}

Vt100Command& Vt100Command::BgColor(Color color) {
  return PrintFormat("\033[48;5;{}m", color);
}

Vt100Command& Vt100Command::SetAttr(Attr attr) {
  return PrintFormat("\033[38;5;{};48;5;{}m", attr.color, attr.background);
}

Vt100Command& Vt100Command::ScrollUp() { return Print("\033M"); }

Vt100Command& Vt100Command::ScrollDown() { return Print("\033D"); }

Vt100Command& Vt100Command::Send() {
  Check(!buf_.empty(), "Sending empty Vt100Command.");

  auto len = buf_.size();
  // Use the low-level write instead of one from standard library in case we
  // want to do in a signal handler (has something to do with the reentrancy of
  // writes from standard library).
  auto res = write(STDOUT_FILENO, buf_.data(), len);
  Check(res == len, "Linux write() didn't write everything.");

  auto capacity_before = buf_.capacity();
  buf_.clear();
  Check(buf_.capacity() == capacity_before,
        "std::string::clear() shrunk the buffer.");

  return *this;
}

}  // namespace e