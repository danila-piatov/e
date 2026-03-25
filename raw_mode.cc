#include "raw_mode.h"

#include <sys/termios.h>
#include <unistd.h>

#include "util.h"
#include "vt100.h"

namespace e {

termios original_termios{};
Vt100Command cleanup_command;

RawModeGuard::RawModeGuard() {
  CheckSyscall(tcgetattr(STDIN_FILENO, &original_termios));

  termios raw = original_termios;

  cfmakeraw(&raw);

  // raw.c_cflag |= CS8;  // Enable 8-bit chars.
  //
  // raw.c_iflag &= ~BRKINT;
  // raw.c_iflag &= ~ICRNL;  // Fix Ctrl-M.
  // raw.c_iflag &= ~INPCK;
  // raw.c_iflag &= ~ISTRIP;
  // raw.c_iflag &= ~IXON;  // Disable Ctrl-S/Ctrl-Q flow control.
  //
  // raw.c_oflag &= ~OPOST;  // Turn off output processing.
  //
  // raw.c_lflag &= ~ECHO;    // Disable echo.
  // raw.c_lflag &= ~ICANON;  // Read byte-by-byte.
  // raw.c_lflag &= ~IEXTEN;  // Disable Ctrl-V.
  // raw.c_lflag &= ~ISIG;    // Disable Ctrl-C/Z signal generation.
  // // Timeout
  raw.c_cc[VMIN] = 1;  // minimum number of bytes that should be received when
  // the read() function returns successfully.
  raw.c_cc[VTIME] = 0;  // timer of 0.1 second granularity that is used to time
  // out bursty and short-term data transmissions.

  CheckSyscall(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw));

  Vt100Command().AlternativeBufferOn().HideCursor().Send();

  // To avoid allocating memory in the destructor (although this only made sense
  // when we were quitting the RAW mode in a signal handler), prepare the
  // cleanup command beforehand.
  cleanup_command./*SetScrollRegion(1, 99999).*/ AlternativeBufferOff()
      .ShowCursor();
}

RawModeGuard::~RawModeGuard() {
  cleanup_command.Send();
  CheckSyscall(tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios));
}

}  // namespace e
