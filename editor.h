#pragma once

#include "terminal.h"

namespace e {

class Editor : public Window {
 public:
  explicit Editor();

 protected:
  void OnResize() final;
  void OnKeyPressed(char ch) override;

private:
  void RerenderText();

  std::string text_;
};

}  // namespace e
