cmake_minimum_required(VERSION 3.5)
project(btft)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


include_directories(include)
add_executable(${PROJECT_NAME} src/main.cpp)
