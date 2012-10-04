project(teachreg)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(dbase)
set(dbase_SRCS  dbase/htmlgenerator.cpp dbase/databaseteachreg.cpp dbase/group.cpp dbase/student.cpp)

set(teachreg_SRCS ${dbase_SRCS} adddatedialog.cpp databasemanager.cpp createdbdialog.cpp teachregmanager.cpp subgroupsdialog.cpp studentlistdialog.cpp teachreg.cpp main.cpp)
set(teachreg_UIS main_window.ui studentlistdlg.ui subgroupsdlg.ui createdbdlg.ui adddatedlg.ui)
set(teachreg_QRCS teachreg.qrc)

qt4_automoc(${teachreg_SRCS})
QT4_WRAP_UI(UI_HEADERS ${teachreg_UIS})
QT4_ADD_RESOURCES(QRC_SOURCES ${teachreg_QRCS})

add_executable(teachreg ${teachreg_SRCS} ${UI_HEADERS} ${QRC_SOURCES})
target_link_libraries(teachreg ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTSQL_LIBRARY})
