cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_BUILD_TYPE "Release")

project(CHIP-8 VERSION 0.0.1)

add_subdirectory(deps)

file(GLOB_RECURSE CHIP8_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE CHIP8_HDR "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_executable(CHIP-8 main.cpp ${CHIP8_SRC} ${CHIP8_HDR})

target_include_directories(CHIP-8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(CHIP-8 PUBLIC engine)

add_custom_target(
  test
  DEPENDS CHIP-8
  COMMAND ./CHIP-8 roms/TETRIS)
