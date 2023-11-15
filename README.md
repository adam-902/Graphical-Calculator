# Graphical-Calculator
Graphical Calculator capable of performing basic arithmetic, basic trigonometric operations as well as plotting graphs


**REQUIREMENTS**:
GTK TOOLKIT
GNUPLOT
CMake
CMake extension on VS Code
CMake Tools extension on VS Code
Gnuplot extension by mammothb
Gnuplot extension by Mario Schwalbe



**Ensure proper installation of CMake and configuration of Cmake extension on VS Code (Extensions->CMake Tools->Build directory->location of your CMake build folder)**

**CMakeLists.txt:**

cmake_minimum_required(VERSION 3.28)
project(GTKTutorial)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(GTK3_INCLUDE_DIRS "C:/msys64/mingw64/include/")
set(GTK3_LIBRARY_DIRS "C:/msys64/mingw64/lib")
set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconfig")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(Calculator main.c)
target_link_libraries(Calculator ${GTK4_LIBRARIES})


For detailed explanation on GTK installation and VS code setup: https://www.youtube.com/live/TyFXwHklg6A?si=0U2LPSa5ytpTEx35

**Provide the proper gnuplot path (Settings->Extensions->Gnuplot Configuration->your Gnuplot path)**

