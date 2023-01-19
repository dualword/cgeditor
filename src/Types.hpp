#pragma once

#include "CMI.hpp"
#include <string>
#include <unordered_map>

namespace cgeditor {

enum class Property : std::uint32_t {
  None = 0,
  Image = 1,
  Rectangle = 1 << 1,
  Text = 1 << 2,
  On = 1 << 3,
  Move = 1 << 4,
  Margin = 1 << 5,
  Menuitem = 1 << 6, // Is it a menu item
  Comment = 1 << 7,
  Black = 1 << 8, // Is it a move for black
  Scrollbar = 1 << 9,
  Horizontal = 1 << 10,  // Is it an horizontal scrollbar
  Scrollbarbg = 1 << 11, // Is it the background of the scrollbar
  Button = 1 << 12,      // Is it a button
  Dots = 1 << 13,        // Move dots
  Movenumber = 1 << 14,
  Current = 1 << 15,
  Mouseover = 1 << 16, // Set on every element where mouse is over
  Pawn = 1 << 17,
  Knight = 1 << 18,
  Bishop = 1 << 19,
  Rook = 1 << 20,
  Queen = 1 << 21,
  King = 1 << 22,
  Nag = 1 << 23
};
Property operator|(Property lhs, Property rhs);
Property &operator|=(Property &lhs, Property rhs);
bool operator&(Property lhs, Property rhs);

class Element {
public:
  Property prop = Property::None;
  std::string text;
  double x, y;
  double width, height;
  /// @brief Should element be scrolled
  bool ShouldApplyScroll = false;
  /// @brief For margin bar to avoid scrolling it vertically
  bool IgnoreScrollY = false;
  /// @brief Check if a given point is over the element
  bool IsOver(const double &X, const double &Y) const;
};

typedef struct Event {
  enum Type { CommentSelected, Promote, Delete, SetAsMainline, Goto, None };
  Type type = None;
  /// @brief Move related to the event
  CMI::HalfMove *move = nullptr;
} Event;

/**
 * @brief Chess Game Editor status
 * Various parameters that can be tuned by the user.
 * The user should manually set mouse event boolean
 * for the editor to work properly
 */
typedef struct Status {
  double MouseX = 0, MouseY = 0;
  double LastMouseClicX = 0, LastMouseClicY = 0;
  double CanvasWidth, CanvasHeight;
  double MenuItemWidth = 150, MenuItemHeight = 50;
  double MoveWidth = 100, MoveHeight = 50;
  double NagWidth = 25, NagHeight = MoveHeight;
  double NagRightMargin = 0;
  double MarginBarWidth = 50;
  double ScrollbarWidth = 30;
  /// @brief The following is not very accuracy but at least work for computing comments bounding boxes and placement
  double CommentCharWidth=9, CommentCharHeight=24;
  /// @brief The following is not a "precise" padding since we add online a multiple of MoveHeight
  std::uint8_t CommentPadding=5;
  /// @brief How many char per line for comments
  std::uint16_t CommentCharPerLine=50;
  double MoveIconWidth = 25;
  /// @brief Ask the editor to scroll for a specific amout of pixels
  double EventVScroll = 0, EventHScroll = 0;
  /// @brief Amount of pixel to scroll elements
  double ScrollX = 0, ScrollY = 0;
  /// @brief Set according to mouse events
  bool LeftClick, RightClick;
  /// @brief Can be use to close the menu
  bool IsMenuOpen = false;
  bool UseMoveIcons = false;
  double MoveTableMaxX = 0, MoveTableMaxY = 0;
  /// @brief User should set it to true when mouse is dragging
  bool IsDrag = false;
  CMI::HalfMove *Moves = nullptr;
  CMI::HalfMove *CurrentMove = nullptr;
  CMI::HalfMove *SelectedMove = nullptr;
  std::vector<Event> Events;
  std::unordered_map<std::uint8_t, std::string> NagTable;
} Status;

} // namespace cgeditor