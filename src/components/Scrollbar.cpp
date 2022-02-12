#include "Scrollbar.hpp"

namespace cgeditor {
Scrollbar::Scrollbar(Status *s, bool IsHorizontal) : Component(s) {
  this->IsHorizontal = IsHorizontal;

  bg.prop = Property::Rectangle | Property::Scrollbarbg;
  bar.prop = Property::Rectangle | Property::Scrollbar;

  if (IsHorizontal) {
    bg.prop |= Property::Horizontal;
    bar.x = 0;
  } else {
    bar.y = 0;
  }

  DragX = 0;
  DragY = 0;
  Trigger = false;
}

void Scrollbar::Refresh() {
  if (IsHorizontal) {
    bg.y = status->CanvasHeight - status->ScrollbarWidth;
    bg.width = status->CanvasWidth - status->ScrollbarWidth;
    bg.height = status->ScrollbarWidth;
    bar.y = bg.y;
  } else {
    bg.x = status->CanvasWidth - status->ScrollbarWidth;
    bg.width = status->CanvasWidth;
    bg.height = status->CanvasHeight - status->ScrollbarWidth;
    bar.x = bg.x;
  }

  bar.width = bg.width;
  bar.height = bg.height;

  // Compute move table canvas
  double MTCanvasHeight = status->CanvasHeight - status->ScrollbarWidth;
  double MTCanvasWidth = status->CanvasWidth - status->ScrollbarWidth;

  bool shouldScroll = false;
  if (!IsHorizontal && status->MoveTableMaxY > MTCanvasHeight) {
    bar.height =
        std::ceil(bg.height * (MTCanvasHeight / status->MoveTableMaxY));
    shouldScroll = true;
  }

  if (IsHorizontal && status->MoveTableMaxX > MTCanvasWidth) {
    bar.width = std::ceil(bg.width * (MTCanvasWidth / status->MoveTableMaxX));
    shouldScroll = true;
  }

  if (shouldScroll) {
    if (IsHorizontal && status->EventHScroll != 0) {
      double percent = status->EventHScroll / status->MoveTableMaxX;
      double maxScroll = bg.width - bar.width;
      bar.x += maxScroll * percent;
      bar.x = std::max(bg.x, bar.x);
      bar.x = std::min(bg.x + maxScroll, bar.x);
      double curScroll = bar.x - bg.x;
      double scrollPercent = curScroll / maxScroll;
      status->ScrollX =
          -(status->MoveTableMaxX - MTCanvasWidth) * scrollPercent;
      status->EventHScroll = 0;
    } else if (status->EventVScroll != 0) {
      double percent = status->EventVScroll / status->MoveTableMaxY;
      double maxScroll = bg.height - bar.height;
      bar.y += maxScroll * percent;
      bar.y = std::max(bg.y, bar.y);
      bar.y = std::min(bg.y + maxScroll, bar.y);
      double curScroll = bar.y - bg.y;
      double scrollPercent = curScroll / maxScroll;
      status->ScrollY =
          -(status->MoveTableMaxY - MTCanvasHeight) * scrollPercent;
      status->EventVScroll = 0;
    } else if (status->LeftClick &&
               bar.IsOver(status->MouseX, status->MouseY)) {
      DragX = bar.x;
      DragY = bar.y;
      Trigger = true;
    } else if (Trigger && status->IsDrag) {
      if (IsHorizontal) {
        bar.x = DragX + (status->MouseX - status->LastMouseClicX);
        bar.x = std::max(bg.x, bar.x);
        double maxScroll = bg.width - bar.width;
        bar.x = std::min(bg.x + maxScroll, bar.x);
        double curScroll = bar.x - bg.x;
        double scrollPercent = curScroll / maxScroll;
        status->ScrollX =
            -(status->MoveTableMaxX - MTCanvasWidth) * scrollPercent;
      } else {
        bar.y = DragY + (status->MouseY - status->LastMouseClicY);
        bar.y = std::max(bg.y, bar.y);
        double maxScroll = bg.height - bar.height;
        bar.y = std::min(bg.y + maxScroll, bar.y);
        double curScroll = bar.y - bg.y;
        double scrollPercent = curScroll / maxScroll;
        status->ScrollY =
            -(status->MoveTableMaxY - MTCanvasHeight) * scrollPercent;
      }
    } else {
      Trigger = false;
    }
  }

  elements.clear();
  elements.push_back(bg);
  elements.push_back(bar);
}

} // namespace cgeditor