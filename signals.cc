#include "signals.h"

#include <atomic>
#include <csignal>
#include <iostream>

#include "util.h"

namespace e {

namespace {

std::atomic<bool> was_quit_signal_received = false;
std::atomic<bool> was_window_resize_signal_received = false;

extern "C" void quit_handler(int) {
  was_quit_signal_received.store(true, std::memory_order_relaxed);
}

extern "C" void window_resize_handler(int) {
  was_window_resize_signal_received.store(true, std::memory_order_relaxed);
}

bool ReadAndResetFlag(std::atomic<bool>& flag) {
  if (!flag.load(std::memory_order_relaxed)) [[likely]] {
    return false;
  }

  return flag.exchange(false, std::memory_order_relaxed);
}

typedef void Handler(int);

void InstallSignalHandler(int signal, Handler* handler) {
  struct sigaction sa{};
  sa.sa_handler = handler;
  CheckSyscall(sigaction(signal, &sa, nullptr));
}

}  // namespace

void InitSignalHandlers() {
  InstallSignalHandler(SIGINT, quit_handler);
  InstallSignalHandler(SIGTERM, quit_handler);
  InstallSignalHandler(SIGWINCH, window_resize_handler);
}

bool WasQuitSignalReceived() {
  return ReadAndResetFlag(was_quit_signal_received);
}

bool WasWindowResizeSignalReceived() {
  return ReadAndResetFlag(was_window_resize_signal_received);
}

}  // namespace e