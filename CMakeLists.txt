cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)

# Set the build type to Debug by default
set(CMAKE_BUILD_TYPE Debug)

# Generate compile_commands.json for Clang tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set compiler flags for debugging
set(CMAKE_CXX_STANDARD 17) # Use C++17 or modify to your desired standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wpedantic") # Add debug flags and warnings
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

project(ProjectZed)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(ProjectZed src/main.cpp src/Renderer.cpp src/VertexBuffer.cpp src/IndexBuffer.cpp src/VertexArray.cpp src/VertexBufferLayout.cpp src/Shader.cpp)
target_link_libraries(ProjectZed OpenGL::GL glfw GLEW::GLEW)
