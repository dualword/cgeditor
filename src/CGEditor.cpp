#include "CGEditor.hpp"
#include <iostream>
namespace cgeditor {

CGEditor::CGEditor() {
  SBV = new Scrollbar(&status, false);
  SBH = new Scrollbar(&status, true);
  MT = new MoveTable(&status);
  MA = new Margin(&status);
  ME = new Menu(&status);
}

CGEditor::~CGEditor() {
  delete SBV;
  delete SBH;
  delete MT;
  delete MA;
  delete ME;
}

void CGEditor::Draw() {
  bool ShoudUpdateMouse = false;
  if (status.LeftClick || status.RightClick) {
    ShoudUpdateMouse = true;
  }

  // Should be refreshed before Scrollbar!
  // To update status.MoveTableMaxX and status.MoveTableMaxY
  MA->Refresh();
  MT->Refresh();
  MA->DrawMargin(MT->GetVariationsMarging());
  SBV->Refresh();
  SBH->Refresh();
  ME->Refresh();

  // Order matter
  DrawComponent(MA);
  DrawComponent(MT);
  DrawComponent(SBV);
  DrawComponent(SBH);
  DrawComponent(ME);

  // Handle events
  for (Event &e : status.Events) {
    HandleEvent(e);
  }
  status.Events.clear();

  // Update mouse events
  status.LeftClick = false;
  status.RightClick = false;
  status.IsDrag = false;
  if (ShoudUpdateMouse) {
    status.LastMouseClicX = status.MouseX;
    status.LastMouseClicY = status.MouseY;
  }
}

void CGEditor::CallDrawElement(Element e) {
  // For element that want to expands up to the edge
  if (e.width < 0) {
    if (e.ShouldApplyScroll) {
      e.width =
          status.CanvasWidth - status.ScrollbarWidth - (e.x + status.ScrollX);
    } else {
      e.width = status.CanvasWidth - status.ScrollbarWidth - e.x;
    }
    if (e.width < 0) {
      e.width *= -1;
    }
  }

  // Apply scroll
  if (e.ShouldApplyScroll) {
    e.x += status.ScrollX;
    if (!e.IgnoreScrollY) {
      e.y += status.ScrollY;
    }
  }

  // Check if element visible if not just leave
  if((e.x+e.width)<0 || status.CanvasWidth < e.x)
    return;
  if((e.y+e.height)<0 || status.CanvasHeight < e.y)
    return;
  
  // Check if mouse over
  if (e.IsOver(status.MouseX, status.MouseY)) {
    e.prop |= Property::Mouseover;
  }

  DrawElement(e);
}

void CGEditor::DrawComponent(Component *c) {
  for (Element &e : c->GetElements()) {
    CallDrawElement(e);
  }
}

} // namespace cgeditor