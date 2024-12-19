cmake_minimum_required(VERSION 3.15)

# Project declaration
project(sqlmate)

# Automatically find all .cpp and .hpp files in the project directory
file(GLOB_RECURSE LIBRARY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE LIBRARY_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)


# Add the library
add_library(sqlmate SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})

target_include_directories(sqlmate PUBLIC include)

# Link SQLite3 library
target_link_libraries(sqlmate LINK_PUBLIC sqlite3)
