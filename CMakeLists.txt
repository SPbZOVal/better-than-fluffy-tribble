cmake_minimum_required(VERSION 3.5)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

if (POLICY CMP0135)
    set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
    cmake_policy(SET CMP0135 NEW)
endif ()

project(btft LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Antlr)

add_library(btft_obj OBJECT)
set(BTFT_TARGET btft_obj)

target_include_directories(${BTFT_TARGET} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_subdirectory(src)

add_executable(${PROJECT_NAME}
        $<TARGET_OBJECTS:btft_obj>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${BTFT_TARGET}
)

btft_setup_antlr(${BTFT_TARGET})
