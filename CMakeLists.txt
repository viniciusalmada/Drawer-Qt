cmake_minimum_required(VERSION 3.5)

project(DrawerQtPainter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH /opt/qt/5.12.10/gcc_64)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIBS GLEW GL)

include_directories(include)

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(DrawerQtPainter
		main.cpp
		mainwindow.cpp
		mainwindow.h
		mainwindow.ui
		customqwidget.cpp customqwidget.h)

target_link_libraries(DrawerQtPainter PRIVATE Qt5::Widgets ${LIBS})
