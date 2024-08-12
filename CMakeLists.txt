
cmake_minimum_required(VERSION 3.1...3.29)

set(CXX_STANDARD 23)

project(
  QuakMan
  VERSION 1.0
  LANGUAGES CXX)

add_subdirectory(src)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR})