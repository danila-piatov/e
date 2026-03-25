#pragma once

namespace e {

class RawModeGuard {
 public:
  explicit RawModeGuard();

  ~RawModeGuard();
};

}  // namespace e
