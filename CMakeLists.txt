cmake_minimum_required(VERSION 3.12)

project(ShootingGame CXX)

set(CMAKE_CXX_STANDARD 17)  # C++ 17

set(FASE_BUILD_EXAMPLES OFF CACHE BOOL "")
set(FASE_USE_CPP17 ON CACHE BOOL "")

add_subdirectory(fase)

add_executable(shooting_game
	src/functions.cpp
	src/main.cpp
)

target_include_directories(shooting_game PUBLIC fase/src)

target_link_libraries(shooting_game fase)
