cmake_minimum_required(VERSION 3.18)
project(cgeditor)

# Includes
set(CGEDITOR_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/includes) # For conveniance
set(CGEDITOR_INCLUDE_DIR ${CGEDITOR_INCLUDE_DIR} PARENT_SCOPE) # To be used by other projects with add_subdirectory()
file(MAKE_DIRECTORY ${CGEDITOR_INCLUDE_DIR})
# Copy all includes
file(GLOB_RECURSE HEADER_FILES src/*.hpp)
file(MAKE_DIRECTORY ${CGEDITOR_INCLUDE_DIR}/components)
foreach(FILE ${HEADER_FILES})
file(RELATIVE_PATH FILE_SUBDIR ${CMAKE_CURRENT_SOURCE_DIR}/src ${FILE})
configure_file(${FILE} ${CGEDITOR_INCLUDE_DIR}/${FILE_SUBDIR} COPYONLY)
endforeach(FILE ${HEADER_FILES})

# Compile cgeditor
include_directories(src)
file(GLOB_RECURSE SRC_CPP_FILES src/*.cpp)
add_library(cgeditor SHARED ${SRC_CPP_FILES})

# Examples
set(COMPILE_EXAMPLES FALSE CACHE BOOL "Compiling included examples")
if(COMPILE_EXAMPLES)
    add_subdirectory("examples/wxWidgets/")
endif()
