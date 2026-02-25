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

target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${BTFT_TARGET}
)

btft_setup_antlr(${BTFT_TARGET})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_target(format
    COMMAND find ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include -name '*.cpp' -o -name '*.hpp' -o -name '*.h' -o -name '*.c' | xargs clang-format-15 -i -style=file
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format on all source files..."
)

add_custom_target(format-check
    COMMAND find ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include -name '*.cpp' -o -name '*.hpp' -o -name '*.h' -o -name '*.c' | xargs clang-format-15 --dry-run -Werror -style=file
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Checking code formatting..."
)


add_custom_target(tidy
    COMMAND find ${CMAKE_SOURCE_DIR}/src -name '*.cpp' -o -name '*.c' -not -path "${CMAKE_SOURCE_DIR}/src/parser" | xargs -I {} clang-tidy-15 -p ${CMAKE_BINARY_DIR} {} -- -Iinclude/ || true
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-tidy on source files..."
)

add_custom_target(test
    COMMAND ${CMAKE_SOURCE_DIR}/test/run_all_tests.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running integration tests..."
    DEPENDS ${PROJECT_NAME}
)

