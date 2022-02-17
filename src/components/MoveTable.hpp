#include "Component.hpp"
#include <cmath>

#define IS_VISIBLE(e)                                                          \
  (((e.x + status->ScrollX) >= 0 &&                                            \
    ((e.x + status->ScrollX) <= status->CanvasWidth) &&                        \
    (e.y + status->ScrollY) >= 0 &&                                            \
    ((e.y + status->ScrollY) <= status->CanvasHeight)) ||                      \
   ((e.x + e.width + status->ScrollX) >= 0 &&                                  \
    ((e.x + e.width + status->ScrollX) <= status->CanvasWidth) &&              \
    (e.y + e.height + status->ScrollY) >= 0 &&                                 \
    ((e.y + e.height + status->ScrollY) <= status->CanvasHeight)))

#define DRAW_DOTS(XX, YY)                                                      \
  {                                                                            \
    Element e;                                                                 \
    e.prop = Property::Text | Property::Dots;                                  \
    e.x = (XX);                                                                \
    e.y = (YY);                                                                \
    e.width = status->MoveWidth;                                               \
    e.height = status->MoveHeight;                                             \
    e.text = "...";                                                            \
    e.ShouldApplyScroll = true;                                                \
    elements.push_back(e);                                                     \
  }

#define DRAW_NB_(XX, YY, NB)                                                   \
  Element ln;                                                                  \
  ln.prop = Property::Text | Property::Movenumber;                             \
  ln.text = std::to_string(NB);                                                \
  ln.x = (XX);                                                                 \
  ln.y = (YY);                                                                 \
  ln.width = status->MarginBarWidth;                                           \
  ln.height = status->MoveHeight;                                              \
  ln.ShouldApplyScroll = true;                                                 \
  elements.push_back(ln);

#define DRAW_NB(XX, YY, NB)                                                    \
  { DRAW_NB_(XX, YY, NB); }

#define DRAW_NB_VAR(XX, YY, NB)                                                \
  {                                                                            \
    DRAW_NB_(XX, YY, NB);                                                      \
    VariationMargins.push_back(ln);                                            \
  }

namespace cgeditor {
class MoveTable : public Component {
  std::uint32_t UpdateMoves(CGEHalfMove *, std::uint32_t, std::uint32_t,bool only_black);
  std::int32_t CurrentMove;
  std::vector<Element> VariationMargins;
  bool IsMouseOver(const Element &e) const;
  std::uint32_t DrawComment(CGEHalfMove *m, std::uint32_t line, std::uint32_t indent,
                           const Element &move_bound, const char &indent_black);
  std::uint32_t DrawVariations(CGEHalfMove *m, std::uint32_t line, std::uint32_t indent,
                              const Element &move_bound,
                              const char &indent_black);

public:
  MoveTable(Status *s);
  void Refresh();
  std::vector<Element> GetVariationsMarging() { return (VariationMargins); }
};
} // namespace cgeditor