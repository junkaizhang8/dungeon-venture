# Create the level editor executable
add_executable(Editor)

# Add the source files
target_sources(Editor PRIVATE Editor.cpp)

# Link the engine library
target_link_libraries(Editor PRIVATE engine)

# Copy the resources
file(COPY res DESTINATION ${CMAKE_BINARY_DIR}/editor)