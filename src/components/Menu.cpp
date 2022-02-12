#include "Menu.hpp"

namespace cgeditor {

Menu::Menu(Status *s) : Component(s), WasOpen(false) {
  entries.push_back("Delete from here");
  entries.push_back("Promote");
  entries.push_back("Set as main line");
}
void Menu::Refresh() {
  if (WasOpen && (status->LeftClick || status->RightClick)) {
    char i = 0;
    for (Element &e : elements) {
      if (e.IsOver(status->MouseX, status->MouseY)) {
        if (i == 0) {
          status->Events.push_back({Event::Type::Delete,status->SelectedMove});
        } else if (i == 1) {
          status->Events.push_back({Event::Type::Promote,status->SelectedMove});
        } else if (i == 2) {
          status->Events.push_back({Event::Type::SetAsMainline,status->SelectedMove});
        }
      }
      i++;
    }
    status->IsMenuOpen = false;
    WasOpen = false;
    elements.clear();
    return;
  }

  elements.clear();
  // Draw menu backward to avoid getting out of the canvas
  bool backwardY =
      (status->MouseY + status->MenuItemHeight * entries.size()) >=
      status->CanvasHeight;
  bool backwardX =
      (status->MouseX + status->MenuItemWidth) >= status->CanvasWidth;

  if (status->IsMenuOpen) {
    char i = 0;
    for (std::string &en : entries) {
      Element e;
      e.prop=Property::Text|Property::Menuitem;
      e.text = en;
      if (backwardX) {
        e.x = (status->MouseX - status->MenuItemWidth);
      } else {
        e.x = status->MouseX;
      }
      if (backwardY) {
        e.y = (status->MouseY - status->MenuItemHeight) -
              i * status->MenuItemHeight;
      } else {
        e.y = status->MouseY + i * status->MenuItemHeight;
      }
      e.width = status->MenuItemWidth;
      e.height = status->MenuItemHeight;
      elements.push_back(e);
      i++;
    }
    WasOpen = true;
  }
}
} // namespace cgeditor