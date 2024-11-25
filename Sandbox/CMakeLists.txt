cmake_minimum_required(VERSION 3.28)

project(Sandbox LANGUAGES CXX)

add_executable(
    Sandbox
    "src/Main.cpp"
)

target_link_libraries(
    Sandbox PRIVATE
    Fluorence
)

add_custom_target(copy_data
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "${CMAKE_CURRENT_BINARY_DIR}/res"
    COMMENT "Copying data"
)

add_dependencies(Sandbox copy_data)