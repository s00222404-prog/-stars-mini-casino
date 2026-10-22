cmake_minimum_required(VERSION 3.16)
project(StarPlay LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib 5.0 REQUIRED)

add_executable(star-play src/main.cpp)
target_link_libraries(star-play PRIVATE raylib)

if(MSVC)
  target_compile_options(star-play PRIVATE /W4)
else()
  target_compile_options(star-play PRIVATE -Wall -Wextra -Wpedantic)
endif()
