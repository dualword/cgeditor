#pragma once

#include <string>
#include <vector>

namespace cgeditor {

/**
 * @brief Move (mainlines and variations) displayed in the editor
 *
 */
class CGEHalfMove {
  /// @brief Comment linked to the move
  std::string comment;
  /// @brief Number of line in @a comment
  std::uint16_t NbLineComment = 0;

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

  CGEHalfMove *MainLine;
  CGEHalfMove *Parent;
  bool IsBlack;
  /// @brief Says if variations of that move must be drawn
  bool Folded = false;
  /// @brief Says if this move must be drawn
  bool Hide = false;
  /// @brief Variations of the move
  std::vector<CGEHalfMove *> variations;

  /// @brief Set comment and update number of lines
  void SetComment(const std::string &c);
  /// @brief Get current value of comment
  std::string GetComment();
  /// @brief Get number of lines in comment
  std::uint16_t GetNbLineComment();
  /// @brief Remove a move from the MainLine and/or variations
  void RemoveChild(CGEHalfMove *m);
};
} // namespace cgeditor