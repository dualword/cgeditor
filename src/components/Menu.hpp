#include "Component.hpp"

namespace cgeditor {
class Menu : public Component {
  std::vector<std::string> entries;
  /// @brief Set to true if the menu was open during the last editor draw
  bool WasOpen;
public:
  Menu(Status *s);
  void Refresh();
};
} // namespace cgeditor