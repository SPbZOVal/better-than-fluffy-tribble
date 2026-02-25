target_sources(${BTFT_TARGET} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/channel.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/executor.cpp"
)

add_subdirectory(commands)