#include "Component.hpp"

namespace cgeditor {
class Margin : public Component {

public:
  Margin(Status *s);
  void Refresh();
  void DrawMargin(Element e);
  void DrawMargin(std::vector<Element> elts);
};
} // namespace cgeditor