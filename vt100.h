#pragma once

#include <format>
#include <cstdint>
#include <string>

namespace e {

using Color = uint8_t;  // The xterm-256 color.

constexpr Color Rgb6(int r, int g, int b) {
  // Check(0 <= r && r < 6);
  // Check(0 <= g && g < 6);
  // Check(0 <= b && b < 6);
  return 16 + 36 * r + 6 * g + b;
}

struct Attr {
  Color color;
  Color background;
};

constexpr auto kLightGray = Rgb6(4, 4, 4);
constexpr auto kWhite = Rgb6(5, 5, 5);
constexpr auto kBlack = Rgb6(0, 0, 0);

constexpr Attr kLightGrayOnBlack = {.color = kLightGray, .background = kBlack};

class Vt100Command {
 public:
  Vt100Command& Reserve(int capacity);

  Vt100Command& AlternativeBufferOn();
  Vt100Command& AlternativeBufferOff();
  Vt100Command& StartSyncUpdate();
  Vt100Command& EndSyncUpdate();
  Vt100Command& HideCursor();
  Vt100Command& ShowCursor();
  Vt100Command& TruncateLine();
  Vt100Command& ClearScreen();
  Vt100Command& ClearScrollbackBuffer();
  Vt100Command& ResetStyle();

  Vt100Command& MoveTo(int row, int col);
  Vt100Command& SetScrollRegion(int top, int bottom);
  Vt100Command& TextColor(Color color);
  Vt100Command& BgColor(Color color);
  Vt100Command& SetAttr(Attr attr);

  Vt100Command& ScrollUp();
  Vt100Command& ScrollDown();

  Vt100Command& Print(char ch) {
    buf_ += ch;
    return *this;
  }

  Vt100Command& Print(std::string_view str) {
    buf_ += str;
    return *this;
  }

  Vt100Command& Send();

  template <typename... Args>
  Vt100Command& PrintFormat(std::format_string<Args...> fmt, Args&&... args) {
    std::format_to(std::back_inserter(buf_), fmt, std::forward<Args>(args)...);
    return *this;
  }

 private:
  std::string buf_;
};

}  // namespace e
