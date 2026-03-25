#include <unistd.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>

#include "editor.h"
#include "raw_mode.h"
#include "signals.h"
#include "terminal.h"
#include "util.h"

namespace e {
//
// void HandleKey(char ch) {
//   switch (ch) {
//     case 17:  // Ctrl-Q
//       raise(SIGINT);
//       break;
//     //
//     // case 3:  // Ctrl-C (Copy)
//     // {
//     //   // // Mock Copy Action
//     //   // // out_buffer.clear();
//     //   // out_buffer.MoveTo(cfg.rows, 1);
//     //   // out_buffer.Print("\033[7m [COPIED TO CLIPBOARD] \033[K\033[m");
//     //   // out_buffer.Send();
//     //   // usleep(500000);  // Brief pause to show status
//     // } break;
//     // case 13:  // Enter
//     // case 10:
//     //   // // Move down, handle scrolling
//     //   // if (cfg.cy < cfg.rows - 2) {
//     //   //   cfg.cy++;
//     //   // } else {
//     //   //   // We are at the bottom of the scroll region.
//     //   //   // Send newline to force hardware scroll of the region.
//     //   //   // We do not increment cy (cursor stays at bottom visual row)
//     //   //   write(STDOUT_FILENO, "\n", 1);
//     //   //   cfg.cx = 1;
//     //   //   return;  // Return early, screen handled by kernel
//     //   // }
//     //   // cfg.cx = 1;
//     //   break;
//     // case 127:  // Backspace
//     //   // if (cfg.cx > 1) {
//     //   //   cfg.cx--;
//     //   //   write(STDOUT_FILENO, "\b \b", 3);  // Destructive backspace
//     //   // }
//     //   return;  // Direct echo handled
//     default:
//
//       if (!iscntrl(ch)) {
//         // // Simple echo logic
//         // write(STDOUT_FILENO, &c, 1);
//         // cfg.cx++;
//         // // Basic wrapping
//         // if (cfg.cx > cfg.cols) {
//         //   cfg.cx = 1;
//         //   if (cfg.cy < cfg.rows - 2)
//         //     cfg.cy++;
//         //   else
//         //     write(STDOUT_FILENO, "\n", 1);
//         // }
//       } else {
//         // puts(
//         // std::cout << std::format("C({})", (int)ch) << std::flush;
//       }
//       break;
//   }
// }

class Menu : public Window {
 protected:
  void OnResize() final {
    Line& line = lines(0);
    line.ResizeContent(width());
    line.ClearWithAttr({.color = Rgb6(2, 2, 2), .background = Rgb6(4, 4, 4)});
    for (int i = 1; i < width() - 1; i++) {
      line.cells(i).ch = '-';
    }
    line.cells(0).ch = '|';
    line.cells(width() - 1).ch = '>';
  }

  void OnKeyPressed(char ch) override {}
};

void MainWithExceptions() {
  Check(isatty(STDOUT_FILENO), "This application must be run in a terminal.");

  RawModeGuard r;
  InitSignalHandlers();

  Menu menu;
  Editor editor;
  Menu status;
  Terminal terminal(menu, editor, status);

  while (!WasQuitSignalReceived()) {
    if (WasWindowResizeSignalReceived()) {
      terminal.WindowResized();
      continue;  // Check signals again in case one arrived just now.
    }

    char input[32];
    auto result = read(STDIN_FILENO, &input, std::size(input));
    Check(result != 0, "We configured the terminal to never read zero bytes.");
    if (result == -1) {
      if (errno == EINTR) {
        continue;  // Got a signal, next iteration will handle it.
      }
      CheckSyscall(result);  // Raise an exception.
    }

    for (int i = 0; i < result; i++) {
      char ch = input[i];
      switch (ch) {
        case 17:  // Ctrl-Q
          raise(SIGINT);
          break;
        default:
          editor.KeyPressed(ch);
          terminal.Redraw();
          break;
      }
    }
  }
}

int Main() {
  try {
    MainWithExceptions();
  } catch (const std::exception& ex) {
    std::fputs(ex.what(), stderr);
    std::fputs("\n", stderr);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

}  // namespace e

int main() { return e::Main(); }
