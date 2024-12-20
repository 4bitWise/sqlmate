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

# Generate doxygen documentation
find_package(Doxygen REQUIRED)

set(DOXYGEN_INPUT_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/Database
                        ${CMAKE_CURRENT_SOURCE_DIR}/SQLite
                        ${CMAKE_CURRENT_SOURCE_DIR}/Model
                        ${CMAKE_CURRENT_SOURCE_DIR}/QueryBuilder
)

set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../docs)

set(DOXYGEN_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../Doxyfile)

add_custom_target(doc ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)