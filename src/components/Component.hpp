#pragma once

#include "Types.hpp"
#include <vector>

namespace cgeditor {

class Component {
protected:
  Status *status;
  std::vector<Element> elements;

public:
  Component(Status *s) : status(s){};
  std::vector<Element> GetElements() { return (this->elements); }
  virtual void Refresh() = 0;
};

} // namespace cgeditor