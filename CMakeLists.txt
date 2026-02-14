cmake_minimum_required(VERSION 3.5)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

if (POLICY CMP0135)
    set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
    cmake_policy(SET CMP0135 NEW)
endif ()

project(better_than_fluffy_tribble LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Java REQUIRED COMPONENTS Runtime)

set(ANTLR_JAR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/antlr-4.13.1-complete.jar")
if (NOT EXISTS "${ANTLR_JAR}")
    message(FATAL_ERROR "ANTLR jar not found: ${ANTLR_JAR}")
endif ()

include(FetchContent)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

set(ANTLR4_INSTALL OFF CACHE BOOL "" FORCE)
set(ANTLR4_WITH_STATIC_CRT OFF CACHE BOOL "" FORCE)

set(ANTLR4_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ANTLR4CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ANTLR4CPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        antlr4_runtime
        GIT_REPOSITORY https://github.com/antlr/antlr4.git
        GIT_TAG 4.13.1
)
FetchContent_MakeAvailable(antlr4_runtime)

add_subdirectory(
        ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp
        ${antlr4_runtime_BINARY_DIR}/runtime/Cpp
)

set(GRAMMAR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/grammar")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(GRAMMAR_FILE "${GRAMMAR_DIR}/Shell.g4")

set(GENERATED_SOURCES
        "${GENERATED_DIR}/ShellLexer.cpp"
        "${GENERATED_DIR}/ShellParser.cpp"
)

set(GENERATED_HEADERS
        "${GENERATED_DIR}/ShellLexer.h"
        "${GENERATED_DIR}/ShellParser.h"
        "${GENERATED_DIR}/ShellVisitor.h"
        "${GENERATED_DIR}/ShellBaseVisitor.h"
)

add_custom_command(
        OUTPUT ${GENERATED_SOURCES} ${GENERATED_HEADERS}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GENERATED_DIR}"
        COMMAND "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR_JAR}"
        -Dlanguage=Cpp
        -visitor
        -no-listener
        -o "${GENERATED_DIR}"
        "${GRAMMAR_FILE}"
        DEPENDS "${GRAMMAR_FILE}" "${ANTLR_JAR}"
        VERBATIM
)

add_custom_target(antlr_gen DEPENDS ${GENERATED_SOURCES} ${GENERATED_HEADERS})

include_directories(include)

add_executable(${PROJECT_NAME}
        ${GENERATED_SOURCES}
        src/main.cpp
        src/shell_repl.cpp
        src/antlr_support.cpp
        src/environment.cpp
        src/antlr_parser.cpp
)

add_dependencies(${PROJECT_NAME} antlr_gen)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${GENERATED_DIR}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE antlr4_static)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
        "${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src"
)
