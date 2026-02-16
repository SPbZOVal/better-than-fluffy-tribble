include_guard(GLOBAL)

include(FetchContent)

function(btft_setup_antlr target_name)
    find_package(Java REQUIRED COMPONENTS Runtime)

    set(ANTLR_JAR "${CMAKE_SOURCE_DIR}/third_party/antlr-4.13.1-complete.jar")
    get_filename_component(ANTLR_JAR "${ANTLR_JAR}" ABSOLUTE)

    if(NOT EXISTS "${ANTLR_JAR}")
        message(FATAL_ERROR "ANTLR jar not found: ${ANTLR_JAR}")
    endif()

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

    set(GRAMMAR_DIR "${CMAKE_SOURCE_DIR}/grammar")
    get_filename_component(GRAMMAR_DIR "${GRAMMAR_DIR}" ABSOLUTE)

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

    target_sources(${target_name} PRIVATE ${GENERATED_SOURCES})
    add_dependencies(${target_name} antlr_gen)

    target_include_directories(${target_name} PRIVATE "${GENERATED_DIR}")

    target_link_libraries(${target_name} PRIVATE antlr4_static)

    target_include_directories(${target_name} SYSTEM PRIVATE
            "${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src"
    )
endfunction()
