# Minimum version required
cmake_minimum_required(VERSION 3.15)

# Project declaration
project(app)

#set(MYSQLX_INCLUDE_DIR "/usr/local/mysql/connector-c++-9.1.0/include/")
#set(CPPCON_INCLUDE_DIR "/usr/lib/x86_64-linux-gnu/libmysqlcppconn.so")
#set(CPPCON_FLAGS "-lmysqlcppcon")

# Add the executable for the example
add_executable(app main.cpp)

# Add the project folder as a subdirectorys
add_subdirectory(../sqlmate ${CMAKE_BINARY_DIR}/sqlmate_build)

# Link the library from the project folder
target_link_libraries(app PRIVATE sqlmate)

# Include the headers of the library
target_include_directories(app PRIVATE ../app)
