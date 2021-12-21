cmake_minimum_required(VERSION 3.10)
project(chip_8)

add_executable(chip_8 "${PROJECT_SOURCE_DIR}/src/main.cpp")

target_link_libraries(chip_8 sfml-graphics sfml-window sfml-system)