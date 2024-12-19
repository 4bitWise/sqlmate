# Minimum version required
cmake_minimum_required(VERSION 3.15)

# Project declaration
project(app)

# Add the executable for the example
add_executable(app main.cpp)

# Add the project folder as a subdirectorys
add_subdirectory(../sqlmate ${CMAKE_BINARY_DIR}/sqlmate_build)

# Link the library from the project folder
target_link_libraries(app PRIVATE sqlmate)
