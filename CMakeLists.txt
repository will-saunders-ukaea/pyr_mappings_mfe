cmake_minimum_required(VERSION 3.21)

project(
  Foo
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Nektar++ REQUIRED)

# Create executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC Nektar++::nektar++)

