[![license](https://img.shields.io/badge/License-LGPL_v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)

# cgeditor: Chess Game Editor
cgeditor is a dependency-free chess game editor library written in C++. It can be used with any library that provides 2D canvas drawing and mouse/keyboard events.

# Features
- Show move, move number, variations, NAGs, comments etc.
- *Delete*, *Promote* and *Set as main line* menu entries
- Handle pieces icons
- Its graphical appareance is entirely customizable

# Architecture
To run cgeditor 2 classes need to be extended:
- CGEditor (To draw and handle events)
- CGEHalfMove (The data structure displayed by the editor)

See example for more informations.

# Example
An example based on *wxWidgets* is available in the `examples/` folder:
![wxWidgets](examples/wxWidgets/demo.gif)
