#pragma once

#include <source_location>
#include <string_view>

namespace e {

int CheckSyscall(int result,
                 std::source_location loc = std::source_location::current());

void CheckFail(std::string_view message, std::source_location loc);

inline void Check(bool value, std::string_view message,
           std::source_location loc = std::source_location::current()) {
  if (!value) [[unlikely]] {
    CheckFail(message, loc);
  }
}

inline void Check(bool value,
                  std::source_location loc = std::source_location::current()) {
  Check(value, "(No reason given).", loc);
}

}  // namespace e