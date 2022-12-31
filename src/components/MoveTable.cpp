#include "MoveTable.hpp"
#include <iostream>

namespace cgeditor {

MoveTable::MoveTable(Status *s) : Component(s) {}

void MoveTable::Refresh() {
  status->MoveTableMaxX = 0;
  status->MoveTableMaxY = 0;
  elements.clear();
  VariationMargins.clear();
  CurrentMove = -1; // No current move by default
  if (status->Moves != NULL) {
    UpdateMoves(status->Moves, 0, 0, status->Moves->IsBlack);
    // We only set the type after the call to UpdateMoves()
    // This way only a single move will be the current move
    if (CurrentMove >= 0) {
      if (status->UseMoveIcons) {
        elements[CurrentMove].prop |= Property::Current;
        elements[CurrentMove + 1].prop |= Property::Current;
      } else {
        elements[CurrentMove].prop |= Property::Current;
      }
    }
  } else {
    Element e;
    e.prop = Property::Text;
    e.text = "No move to show";
    e.x = status->MarginBarWidth;
    e.y = 0;
    e.height = status->MoveHeight;
    e.width = -1;
    elements.push_back(e);
  }
}

bool MoveTable::IsMouseOver(const Element &e) const {
  // Check if we clicked on scroll bars
  if (status->IsMenuOpen ||
      status->MouseX >= (status->CanvasWidth - status->ScrollbarWidth) ||
      status->MouseY >= (status->CanvasHeight - status->ScrollbarWidth)) {
    return (false);
  }
  return (e.IsOver(status->MouseX - status->ScrollX,
                   status->MouseY - status->ScrollY));
}

std::uint32_t MoveTable::UpdateMoves(CGEHalfMove *m, std::uint32_t line,
                                     std::uint32_t indent, bool only_black) {

  //---------- Check black or white ----------
  char indent_black = 0;
  if (m->IsBlack) {
    indent_black++;
  }

  //---------- Create temporary move surrounding area ----------
  Element move_bound;
  move_bound.prop = Property::Move;
  if (m->IsBlack) {
    move_bound.prop |= Property::Black;
  }
  move_bound.x = status->MarginBarWidth +
                 status->MoveWidth * (indent + indent_black) +
                 ((indent + 1) / 2 * status->MarginBarWidth);
  move_bound.y = status->MoveHeight * line;
  move_bound.width = status->MoveWidth;
  move_bound.height = status->MoveHeight;
  move_bound.text = m->move;
  move_bound.ShouldApplyScroll = true;
  bool isMouseOver = IsMouseOver(move_bound);

  //---------- Update current focus move ----------
  if (isMouseOver) {
    if (status->LeftClick) {
      if (!status->IsMenuOpen) {
        status->Events.push_back({Event::Type::Goto, m});
        status->CurrentMove = m;
      }
    } else if (status->RightClick) {
      status->IsMenuOpen = true;
      status->SelectedMove = m;
    }
  }

  //---------- Check if current move is focused ----------
  if (status->CurrentMove == m) {
    CurrentMove = elements.size();
  }

  //---------- Draw move ----------
  if (status->UseMoveIcons) {
    // Image
    Element img;
    img.prop = move_bound.prop | Property::Image;
    img.x = move_bound.x;
    img.y = status->MoveHeight * line;
    img.width = status->MoveIconWidth;
    img.height = status->MoveHeight;
    img.ShouldApplyScroll = true;
    // Move
    Element e;
    e.prop = move_bound.prop | Property::Text;
    e.text = m->move;
    if (m->move.size() > 0) {
      char c = m->move[0];
      if (!(c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' ||
            c == 'f' || c == 'g' || c == 'h' || c == 'O' || c == '0')) {
        e.text = m->move.substr(1, m->move.size());
        if (c == 'N') {
          img.prop |= Property::Knight;
        } else if (c == 'B') {
          img.prop |= Property::Bishop;
        } else if (c == 'R') {
          img.prop |= Property::Rook;
        } else if (c == 'Q') {
          img.prop |= Property::Queen;
        } else {
          img.prop |= Property::King;
        }
      } else {
        img.prop |= Property::Pawn;
      }
    }
    e.x = status->MoveIconWidth + move_bound.x;
    e.y = status->MoveHeight * line;
    e.width = status->MoveWidth - status->MoveIconWidth;
    e.height = status->MoveHeight;
    e.ShouldApplyScroll = true;
    // Add elements:
    elements.push_back(img);
    elements.push_back(e);
  } else {
    move_bound.prop |= Property::Text;
    elements.push_back(move_bound);
  }

  //---------- NAG ----------
  if(m->nag.size()>0){
    Element nag;
    nag.text = m->nag;
    nag.x = move_bound.x + status->MoveWidth - status->NagWidth - status->NagRightMargin;
    nag.y = status->MoveHeight * line;
    nag.width = status->NagWidth;
    nag.height = status->NagHeight;
    nag.prop = move_bound.prop | Property::Text | Property::Nag;
    nag.ShouldApplyScroll = true;
    elements.push_back(nag);
  }

  //---------- Move number in marge or for variation ----------
  if (indent == 0 && (!m->IsBlack || only_black)) {
    DRAW_NB(0, status->MoveHeight * line, m->Number);
  } else if (indent > 0 && (!m->IsBlack || only_black)) {
    if (only_black) {
      DRAW_NB_VAR((move_bound.x - status->MoveWidth) - status->MarginBarWidth,
                  status->MoveHeight * line, m->Number);
    } else {
      DRAW_NB_VAR(move_bound.x - ((indent + 1) / 2 * status->MarginBarWidth),
                  status->MoveHeight * line, m->Number);
    }
  }

  //---------- Draw dots ----------
  if (only_black) {
    DRAW_DOTS(move_bound.x - status->MoveWidth, move_bound.y);
  }

  //---------- Scrolling infos ----------
  if ((move_bound.x + move_bound.width) > status->MoveTableMaxX) {
    status->MoveTableMaxX = move_bound.x + move_bound.width;
  }
  if ((move_bound.y + move_bound.height) > status->MoveTableMaxY) {
    status->MoveTableMaxY = move_bound.y + move_bound.height;
  }

  //---------- Comments ----------
  if (m->comment.size() > 0) {
    line = DrawComment(m, line, indent, move_bound, indent_black);
  }

  //---------- Variations ----------
  if (m->variations.size() > 0) {
    line = DrawVariations(m, line, indent, move_bound, indent_black);
  }

  //---------- Mainline ----------
  if (m->MainLine != NULL) {
    only_black = (m->MainLine->IsBlack &&
                  (m->comment.size() > 0 || m->variations.size()));
    if (m->IsBlack) {
      line = UpdateMoves(m->MainLine, line + 1, indent, only_black);
    } else {
      line = UpdateMoves(m->MainLine, line, indent, only_black);
    }
  }

  return (line);
}

std::uint32_t MoveTable::DrawComment(CGEHalfMove *m, std::uint32_t line,
                                     std::uint32_t indent,
                                     const Element &move_bound,
                                     const char &indent_black) {
  // Show three dots
  if (!m->IsBlack) {
    DRAW_DOTS(status->MarginBarWidth + status->MoveWidth * (indent + 1) +
                  ((indent + 1) / 2 * status->MarginBarWidth),
              status->MoveHeight * line);
  }
  line++; // Goto the right line

  /// ----- Compute comment bounding box values
  int nchar=m->comment.size();
  int nline=ceil((double)nchar/(double)status->CommentCharPerLine);
  std::uint16_t nrow=ceil(((nline*status->CommentCharHeight)+2*status->CommentPadding)/status->MoveHeight);
  int width=status->CommentCharPerLine*status->CommentCharWidth+2*status->CommentPadding;
  int height=nrow*status->MoveHeight;

  // Draw comment background
  Element e;
  e.prop = Property::Rectangle | Property::Comment;
  e.x = move_bound.x -
        (indent_black *
         status->MoveWidth); // status->MarginBarWidth + status->MoveX;
  e.y = status->MoveHeight * line;
  e.width = -1; // Negative width means expands to the edge of the canvas
  e.height = height;
  e.ShouldApplyScroll = true;
  elements.push_back(e);
  // Update scrolling
  if ((e.x + width) > status->MoveTableMaxX) {
    status->MoveTableMaxX = e.x + width;
  }
  if ((e.y + height) > status->MoveTableMaxY) {
    status->MoveTableMaxY = e.y + height;
  }
  // Handle events:
  if (status->LeftClick && IsMouseOver(e)) {
    status->Events.push_back({Event::Type::CommentSelected, m});
  }
  // Now draw each lines of the comment:
  Element l; // One line
  l.prop = Property::Comment|Property::Text;
  l.x=e.x+status->CommentPadding;
  l.y=e.y+status->CommentPadding;
  l.width=width;
  l.height=status->CommentCharHeight;
  l.ShouldApplyScroll = true;
  for(int i=0;i<nline;i++){
    l.text=m->comment.substr(i*status->CommentCharPerLine,status->CommentCharPerLine);
    // Remove leading space:
    if(l.text.size()>2 && l.text[0]==' '){
      l.text=l.text.substr(1,l.text.size());
    }
    elements.push_back(l);
    l.y+=status->CommentCharHeight;
  }
  // Do not forget to add marging before comment if indented:
  if (indent > 0) {
    e.x -= status->MarginBarWidth;
    VariationMargins.push_back(e);
  }
  line += nrow; // Skip right amount of lines
  // Since we already increment line for black later on:
  if (m->IsBlack || m->variations.size() > 0) {
    line--;
  }

  return (line);
}

std::uint32_t MoveTable::DrawVariations(CGEHalfMove *m, std::uint32_t line,
                                        std::uint32_t indent,
                                        const Element &move_bound,
                                        const char &indent_black) {
  // Show three dots next to move if white turn
  if ((m->variations.size() == 0) && !m->IsBlack) {
    DRAW_DOTS(status->MarginBarWidth + status->MoveWidth * (indent + 1),
              status->MoveHeight * line);
  }
  // Show button on the right side of the move
  {
    Element e;
    e.prop = Property::Rectangle | Property::Button;
    e.x = move_bound.x + status->MoveWidth;
    if (!m->IsBlack)
      e.x += status->MoveWidth;
    e.y = move_bound.y + std::ceil(status->MoveHeight / 4);
    e.width = std::ceil(status->MoveHeight / 2);
    e.height = e.width;
    e.ShouldApplyScroll = true;
    if (status->LeftClick && IsMouseOver(e)) {
      m->Folded = !m->Folded;
    }
    if (!m->Folded) {
      e.prop |= Property::On;
    }
    elements.push_back(e);
  }
  if (!m->Folded) {
    for (CGEHalfMove *v : m->variations) {
      // For each variation show show/hide button
      {
        Element e;
        e.prop = Property::Rectangle | Property::Button;
        e.x = (status->MarginBarWidth + status->MoveWidth * indent) +
              status->MoveWidth - std::ceil(status->MoveHeight / 2) -
              std::ceil(status->MoveHeight / 4);
        e.y = (status->MoveHeight * (line + 1)) +
              std::ceil(status->MoveHeight / 4);
        e.width = std::ceil(status->MoveHeight / 2);
        e.height = e.width;
        e.ShouldApplyScroll = true;
        if (status->LeftClick && IsMouseOver(e)) {
          v->Hide = !v->Hide;
        }
        if (!v->Hide) {
          e.prop |= Property::On;
        }
        elements.push_back(e);
      }
      if (!v->Hide) {
        line = UpdateMoves(v, line + 1, indent + 1, v->IsBlack);
      } else {
        line++;
      }
    }
  }
  // New line after variation
  if (m->MainLine != NULL && m->MainLine->IsBlack) {
    line++;
  }
  return (line);
}
} // namespace cgeditor