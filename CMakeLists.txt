project(teachreg)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)
set(TEACHREG_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
add_definitions(-DTEACHREG_VERSION=\"${TEACHREG_VERSION}\")
message(STATUS "")
message(STATUS "TeachReg version: ${TEACHREG_VERSION}")
message(STATUS "")

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(dbase)
set(dbase_SRCS help_browser/helpbrowser.cpp  dbase/htmlgenerator.cpp dbase/databaseteachreg.cpp dbase/group.cpp dbase/student.cpp)

set(teachreg_SRCS ${dbase_SRCS} adddatedialog.cpp databasemanager.cpp createdbdialog.cpp teachregmanager.cpp subgroupsdialog.cpp studentlistdialog.cpp teachreg.cpp main.cpp)
set(teachreg_UIS help_browser/helpform.ui main_window.ui studentlistdlg.ui subgroupsdlg.ui createdbdlg.ui adddatedlg.ui)
set(teachreg_QRCS teachreg.qrc)

qt4_automoc(${teachreg_SRCS})
QT4_WRAP_UI(UI_HEADERS ${teachreg_UIS})
QT4_ADD_RESOURCES(QRC_SOURCES ${teachreg_QRCS})

add_executable(teachreg ${teachreg_SRCS} ${UI_HEADERS} ${QRC_SOURCES})
target_link_libraries(teachreg ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

add_subdirectory(help_browser)