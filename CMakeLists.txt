cmake_minimum_required(VERSION 3.5)
project(lol_better_than_dota)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(SOURCES
  src/main.cpp
  src/executor/executor.cpp
  src/executor/channel.cpp
  src/executor/commands/echo.cpp
)

include_directories(include)
add_executable(${PROJECT_NAME} ${SOURCES})
