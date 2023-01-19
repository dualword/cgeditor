#pragma once

#include "CGEditor.hpp"

/**
 * @brief Create your custom half move class
 * 
 * The implementation of the class should give you
 * an overview of how to keep your move sync with the one of CGEditor
 * 
 */
class MyHalfMove : public CMI::HalfMove {

public:
  MyHalfMove(std::string move);
};

/// @brief Build the example game to use in the editor
MyHalfMove *BuildExampleGame();