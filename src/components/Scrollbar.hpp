#include "Component.hpp"
#include <algorithm>    // std::max
#include <cmath>

namespace cgeditor {
class Scrollbar : public Component {
  /// @brief Set to true if it is the horizontal scrollbar
  bool IsHorizontal;
  Element bg,bar;
  double DragY,DragX;
  bool Trigger;
public:
  Scrollbar(Status* s,bool IsHorizontal);
  void Refresh();
};
} // namespace cgeditor