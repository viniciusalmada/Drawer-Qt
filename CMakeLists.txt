cmake_minimum_required(VERSION 3.5)

project(DrawerQtPainter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
if (UNIX)
    set(CMAKE_PREFIX_PATH /opt/qt/5.12.10/gcc_64)
    set(CMAKE_C_COMPILER clang)
elseif (WIN32)
    set(CMAKE_PREFIX_PATH C:\\Qt\\Qt5.4.0\\5.4\\mscv2013_opengl)
endif ()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(include)

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(DrawerQtPainter
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        canvasqwidget.cpp canvasqwidget.h
        manager/model.cpp manager/model.h
        geom/point.h
        geom/curve.h geom/curve.cpp
        geom/line.cpp geom/line.h
        geom/cubicbezier.h geom/cubicbezier.cpp
        geom/box.h
        manager/grid.cpp manager/grid.h
        manager/curvecollector.cpp manager/curvecollector.h
        geom/geomutils.h geom/geomutils.cpp
        math/matrix.cpp math/matrix.h
        math/smatrix.cpp math/smatrix.h
        mesh/delaunaymodel.cpp mesh/delaunaymodel.h
        topo/hedsimplified.h topo/hedsimplified.cpp geom/region.cpp geom/region.h)

target_link_libraries(DrawerQtPainter PRIVATE Qt5::Widgets)
