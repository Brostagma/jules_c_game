# CMake minimum version requirement
cmake_minimum_required(VERSION 3.10)

# Project definition
project(CosmicShards VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- SDL2 Libraries ---
# Set the path where CMake will look for our libraries.
# We tell CMake to look in the 'libs' directory.
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/libs)

# Use find_package to locate SDL2 headers and libraries.
# This will find SDL2 and SDL2_ttf in the CMAKE_PREFIX_PATH.
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# --- Project Files ---
# Set the directory for our header files.
include_directories(include)

# Gather all our source files to be compiled.
file(GLOB_RECURSE SOURCES "src/*.cpp")

# --- Executable Definition ---
# Define the executable target for our game.
add_executable(CosmicShards ${SOURCES})

# Link the SDL2 and SDL2_ttf libraries to our executable.
# These are imported targets created by CMake's find modules.
target_link_libraries(CosmicShards PRIVATE SDL2::SDL2 SDL2::SDL2_ttf)

# --- Windows Specific Settings ---
# On Windows, create a windowed application instead of a console one.
if(WIN32)
    set_target_properties(CosmicShards PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# --- Installation ---
# An optional install step that places the executable in a 'bin' directory
# and copies the necessary SDL2.dll for distribution.
install(TARGETS CosmicShards DESTINATION bin)
if(WIN32)
    install(FILES ${SDL2_LIBRARY_DLL} DESTINATION bin)
endif()

message(STATUS "Project setup complete. Executable 'CosmicShards' will be created.")
message(STATUS "To build, run: cmake --build .")
message(STATUS "After building, the executable will be in the build directory, or in 'build/bin' if you run the install step.")