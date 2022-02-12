#include "CGEditor.hpp"
#include <vector>

/**
 * @brief Create your custom half move class
 * 
 * The implementation of the class should give you
 * an overview of how to keep your move sync with the one of CGEditor
 * 
 */
class MyHalfMove : public cgeditor::CGEHalfMove {
  MyHalfMove *parent = NULL;
  MyHalfMove *mainline = NULL;
  std::vector<MyHalfMove *> variations;

public:
  MyHalfMove(std::string move);
  ~MyHalfMove();
  /// @brief Add variation to current move
  void AddVariation(MyHalfMove *m);
  /// @brief Remove the specified child from mainline and/or variations
  void RemoveChild(MyHalfMove *m);
  /// @brief Set value of the mailine
  void SetMainline(MyHalfMove *m);
  /// @brief Set this move as mainline
  void SetAsMainline();
  /// @brief Promote the current move and submove
  void Promote();
  /// @brief Check if current half move is within a variation
  bool IsVariation();
  /// @brief Get the root of a variation
  MyHalfMove* GetRoot();
  /// @brief Get parent of the current move
  MyHalfMove* GetParent();
  /// @brief Set parent of the current move
  void SetParent(MyHalfMove *m);
};

/// @brief Build the example game to use in the editor
MyHalfMove *BuildExampleGame();