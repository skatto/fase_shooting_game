cmake_minimum_required(VERSION 3.12)

project(ShootingGame CXX)

set(CMAKE_CXX_STANDARD 17)  # C++ 17

set(FASE_BUILD_THIRD_PARTY ON CACHE BOOL "Build third pary libraries")
set(FASE_BUILD_GLFW ON CACHE BOOL "Build glfw libraries and link")
set(FASE_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set(FASE_BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)
set(FASE_USE_CPP17 ON CACHE BOOL "Use c++17 for further feathers" FORCE)

add_subdirectory(fase)

add_executable(shooting_game
	src/functions.cpp
	src/main.cpp
	src/game.cpp
)

target_include_directories(shooting_game PUBLIC fase/src)

target_link_libraries(shooting_game fase)
