#include "Margin.hpp"

namespace cgeditor {

Margin::Margin(Status *s) : Component(s) {}

void Margin::Refresh() {
  elements.clear();
  Element e;
  e.x = 0;
  e.y = 0;
  e.height = status->CanvasHeight - status->ScrollbarWidth;
  e.ShouldApplyScroll = true;
  e.IgnoreScrollY = true;
  DrawMargin(e);
}

void Margin::DrawMargin(Element e) {
  e.prop=Property::Margin | Property::Rectangle;
  e.width = status->MarginBarWidth;
  elements.push_back(e);
}

void Margin::DrawMargin(const std::vector<Element> &elts) {
  for(Element e:elts){
    DrawMargin(e);
  }
}

} // namespace cgeditor