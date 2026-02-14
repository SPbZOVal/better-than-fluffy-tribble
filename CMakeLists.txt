cmake_minimum_required(VERSION 3.5)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

if(POLICY CMP0135)
  set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
  cmake_policy(SET CMP0135 NEW)
endif()

project(btft LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Antlr)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/shell_repl.cpp
  src/antlr_support.cpp
  src/environment.cpp
  src/antlr_parser.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

btft_setup_antlr(${PROJECT_NAME})
