#include "editor.h"

namespace e {

Editor::Editor() {
  ShowCursor();
}

void Editor::OnResize() {
  for (int i = 0; i < height(); i++) {
    Line& line = lines(i);
    line.SetBackground(Rgb6(0, 0, 0));
  }
}

void Editor::OnKeyPressed(char ch) {
  text_ += ch;
  RerenderText();
}

void Editor::RerenderText() {

  Line& line = lines(0);
  line.ResizeContent(text_.length());
  for (int i = 0; i < text_.length(); i++) {
    line.cells(i).ch = text_[i];
    line.cells(i).attr = kLightGrayOnBlack;
  }

  MoveCursor(0, text_.length());
}

}  // namespace e
