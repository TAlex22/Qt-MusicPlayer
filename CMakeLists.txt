cmake_minimum_required(VERSION 3.5)

project(MusicPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES MusicPlayer_ru_BY.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        Track.h
        Track.cpp
        Playlist.h
        Playlist.cpp
        mainwindow.h
        mainwindow.ui
        logindialog.h
        logindialog.cpp
        logindialog.ui
        resources.qrc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MusicPlayer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MusicPlayer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(MusicPlayer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MusicPlayer
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(MusicPlayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(MusicPlayer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MusicPlayer)
endif()

list(APPEND CMAKE_PREFIX_PATH "/Users/user/Qt/6.3.1/macos/lib/cmake")

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)
find_package(Qt6MultimediaWidgets REQUIRED)

qt_standard_project_setup()

target_link_libraries(MusicPlayer PUBLIC Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(MusicPlayer PUBLIC Qt${QT_VERSION_MAJOR}::MultimediaWidgets)


set_target_properties(MusicPlayer PROPERTIES AUTOMOC TRUE)

target_link_libraries(MusicPlayer
  PUBLIC
   Qt6::Widgets
   Qt6::Core
)

