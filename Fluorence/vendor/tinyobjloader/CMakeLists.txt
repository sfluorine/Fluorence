cmake_minimum_required(VERSION 3.28)

project(tinyobjloader LANGUAGES CXX)

add_library(
    tinyobjloader STATIC
    "src/tiny_obj_loader.cpp"
)

target_include_directories(
    tinyobjloader PUBLIC
    "include"
)