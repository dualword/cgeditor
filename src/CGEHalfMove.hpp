#pragma once

#include <string>
#include <vector>

namespace cgeditor {

/**
 * @brief Move (mainlines and variations) displayed in the editor
 *
 */
class CGEHalfMove {

public:
  CGEHalfMove();
  CGEHalfMove(CGEHalfMove *parent);
  CGEHalfMove(std::string move);

  /// @brief CUrrent move number
  std::uint16_t Number;
  /// @brief Current move value
  std::string move;
  /// @brief Current NAG
  std::string nag;
  /// @brief Comment linked to the move
  std::string comment;

  CGEHalfMove *MainLine;
  CGEHalfMove *Parent;
  bool IsBlack;
  /// @brief Says if variations of that move must be drawn
  bool Folded = false;
  /// @brief Says if this move must be drawn
  bool Hide = false;
  /// @brief Variations of the move
  std::vector<CGEHalfMove *> variations;

  /// @brief Remove a move from the MainLine and/or variations
  void RemoveChild(CGEHalfMove *m);
};
} // namespace cgeditor