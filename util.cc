#include "util.h"

#include <cerrno>
#include <format>
#include <system_error>

namespace e {

namespace {

std::string ToString(std::source_location loc) {
  return std::format("'{}' at {}:{}", loc.function_name(), loc.file_name(),
                     loc.line());
}

}  // namespace

int CheckSyscall(int result, std::source_location loc) {
  if (result == -1) [[unlikely]] {
    auto err = errno;
    throw std::system_error(err, std::system_category(),
                            std::format("Syscall failed in {}", ToString(loc)));
  }
  return result;
}

void CheckFail(std::string_view message, std::source_location loc) {
  throw std::runtime_error(
      std::format("Error in {}: {}", ToString(loc), message));
}

}  // namespace e