#pragma once

#include "Types.hpp"
#include "components/Margin.hpp"
#include "components/Menu.hpp"
#include "components/MoveTable.hpp"
#include "components/Scrollbar.hpp"

#include <string>

namespace cgeditor {

class CGEditor {
  /// @brief Prepare element for drawing and draw
  void CallDrawElement(Element);
  /// @brief Draw all elements of a component
  void DrawComponent(Component *);

  Scrollbar *SBV, *SBH;
  MoveTable *MT;
  Margin *MA;
  Menu *ME;

protected:
  Status status;
  /// @brief Draw the Chess Game Editor on the canvas using current status
  void Draw();
  /// @brief Process the events generated during the drawing
  bool ProcessEvents();
  /// @brief Synchronize the editor cache (must be called when game was modified from outside the editor)
  void SyncCache();
  /// @brief Draw an element on the canvas
  virtual void DrawElement(const Element &) = 0;
  /// @brief Handle event that occured during editor drawing
  virtual void HandleEvent(const Event &) = 0;
  /// @brief Convert NAG id to symbol using the NagTable
  std::string GetNAGSymbol(const std::uint8_t) const;
  /// @brief Convert NAG symbol to id using the NagTable
  std::uint8_t GetNAGId(const std::string&) const;
public:
  CGEditor();
  ~CGEditor();
};
} // namespace cgeditor