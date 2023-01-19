
find_package(wxWidgets COMPONENTS core base REQUIRED)

include(${wxWidgets_USE_FILE})
add_executable(wxwidgets_example main.cpp MyHalfMove.cpp)
target_link_libraries(wxwidgets_example cgeditor ${wxWidgets_LIBRARIES})
target_link_libraries(wxwidgets_example ChessMoveInterface)

