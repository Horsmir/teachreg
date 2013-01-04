project(teachreg)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

if(CMAKE_CROSSCOMPILING)
  set(QT_HEADERS_DIR "/usr/i486-mingw32/include")
  set(QT_LIBRARY_DIR "/usr/i486-mingw32/lib")
endif(CMAKE_CROSSCOMPILING)

if (NOT CMAKE_BUILD_TYPE)
    set ( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)

if (CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    message(STATUS "Debug build")
    add_definitions(-DDEBUG)
else()
    message(STATUS "non-debug build")
    add_definitions(-DNDEBUG)
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

if (NOT CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX /usr)
endif(NOT CMAKE_INSTALL_PREFIX)
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

set(MAJOR_VERSION 0)
set(MINOR_VERSION 2)
set(PATCH_VERSION 2)
set(TEACHREG_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
add_definitions(-DTEACHREG_VERSION=\"${TEACHREG_VERSION}\")
message(STATUS "")
message(STATUS "TeachReg version: ${TEACHREG_VERSION}")
message(STATUS "")

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

configure_file (
  "${PROJECT_SOURCE_DIR}/teachregconfigure.h.in"
  "${PROJECT_BINARY_DIR}/teachregconfigure.h"
  )

add_subdirectory(dbase)
set(dbase_SRCS help_browser/helpbrowser.cpp  dbase/htmlgenerator.cpp dbase/databaseteachreg.cpp dbase/group.cpp dbase/student.cpp)

set(teachreg_SRCS ${dbase_SRCS} optionsdialog.cpp teachmodel.cpp adddatedialog.cpp createdbdialog.cpp teachregmanager.cpp subgroupsdialog.cpp studentlistdialog.cpp teachreg.cpp main.cpp)
set(teachreg_UIS optionsdlg.ui help_browser/helpform.ui main_window.ui studentlistdlg.ui subgroupsdlg.ui createdbdlg.ui adddatedlg.ui)
set(teachreg_QRCS teachreg.qrc)

qt4_automoc(${teachreg_SRCS})
QT4_WRAP_UI(UI_HEADERS ${teachreg_UIS})
QT4_ADD_RESOURCES(QRC_SOURCES ${teachreg_QRCS})

add_executable(teachreg WIN32 ${teachreg_SRCS} ${UI_HEADERS} ${QRC_SOURCES})
target_link_libraries(teachreg ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

add_subdirectory(help_browser)

install(TARGETS teachreg RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES teachreg.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES icons/teachreg.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps)
install(DIRECTORY doc/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/teachreg PATTERN "*~" EXCLUDE)

########## Add make dist target ################
set(CPACK_PACKAGE_VERSION "${TEACHREG_VERSION}")
set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "teachreg-${TEACHREG_VERSION}")
set(CPACK_IGNORE_FILES "/CVS/;/\\\\.git\;/\\\\.svn/;\\\\.swp$;\\\\.#;/#;\\\\.tar.gz$;/CMakeFiles/;CMakeCache.txt;build/;dist/;exp/;config/;data/;build-win32/;.*~;exp-win32/;install-win32/")
set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
include(CPack)
# simulate autotools' "make dist"
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
