target_sources(${BTFT_TARGET} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/shell_repl.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/environment.cpp"
)

add_subdirectory(parser)
add_subdirectory(executor)
