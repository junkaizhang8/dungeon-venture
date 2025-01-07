# Define the source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create a library for the engine
add_library(engine STATIC)

# Add the source files to the library
target_sources(engine PRIVATE ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb/stb_image.cpp)

# Public include directories
target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

# Add ImGui library
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
file(GLOB IMGUI_SOURCES "${IMGUI_DIR}/*.cpp")
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR})
target_link_libraries(imgui PRIVATE glfw libglew_static)

# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Pass the debug flag
if(ENGINE_DEBUG)
    target_compile_definitions(engine PUBLIC ENGINE_DEBUG)
endif()

# Add subdirectories
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glew)

# Link libraries
target_link_libraries(engine PUBLIC
    glfw
    libglew_static
    imgui
)