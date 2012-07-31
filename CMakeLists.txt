project(teachreg)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(teachreg_SRCS teachreg.cpp main.cpp)
qt4_automoc(${teachreg_SRCS})
add_executable(teachreg ${teachreg_SRCS})
target_link_libraries(teachreg ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
