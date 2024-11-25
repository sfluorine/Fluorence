cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(Fluorence LANGUAGES CXX)

add_library(
    Fluorence STATIC
    "src/Fluorence.cpp"
    "src/Renderer/Shader.cpp"
    "src/Renderer/Texture.cpp"
    "src/Renderer/VertexArray.cpp"
    "src/Renderer/VertexBuffer.cpp"
    "src/3D/Model.cpp"
    "src/3D/Camera.cpp"
)

add_subdirectory(vendor)

target_include_directories(Fluorence PUBLIC "include")
target_link_libraries(
    Fluorence PUBLIC
    glad
    glfw
    glm::glm
    spdlog
    stb_image
    tinyobjloader
)