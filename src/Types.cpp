#include "Types.hpp"

namespace cgeditor {

bool Element::IsOver(const double &X, const double &Y) const {
  // Check if no overlap
  if(X<x|| X>(x+width) || Y<y || Y>(y+height))
    return false;
  return true;
}

Property operator|(Property lhs, Property rhs) {
  return static_cast<Property>(
      static_cast<std::underlying_type_t<Property>>(lhs) |
      static_cast<std::underlying_type_t<Property>>(rhs));
}

bool operator&(Property lhs, Property rhs) {
  return (static_cast<std::underlying_type_t<Property>>(lhs) &
          static_cast<std::underlying_type_t<Property>>(rhs));
}

Property &operator|=(Property &lhs, Property rhs) {
  return lhs = static_cast<Property>(
             static_cast<std::underlying_type_t<Property>>(lhs) |
             static_cast<std::underlying_type_t<Property>>(rhs));
}

} // namespace cgeditor