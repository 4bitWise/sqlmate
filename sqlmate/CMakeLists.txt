cmake_minimum_required(VERSION 3.15)

# Project declaration
project(sqlmate)

#set(CPPCON_INCLUDE_DIR "/usr/local/mysql/connector-c++-9.1.0/lib64/debug")
#set(MYSQLX_INCLUDE_DIR "/usr/local/mysql/connector-c++-9.1.0/include/") # "include in .hpp using mysqlx/header.h"
#set(CPPCON_FLAGS "-lmysqlcppcon")

# Automatically find all .cpp and .hpp files in the project directory
file(GLOB_RECURSE LIBRARY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE LIBRARY_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Add the library
add_library(sqlmate SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
#add_executable(executable ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})

# Include directories (for the library headers)
target_include_directories(sqlmate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Optionally, set the output directory for the shared library
set_target_properties(sqlmate PROPERTIES
LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

#target_include_directories(sqlmate PRIVATE ${MYSQLX_INCLUDE_DIR})
#target_link_libraries(sqlmate ${CPPCON_INCLUDE_DIR}/libmysqlcppconnx-static.a)
target_link_libraries(sqlmate LINK_PUBLIC sqlite3)

#target_include_directories(executable PRIVATE ${MYSQLX_INCLUDE_DIR})
#target_link_libraries(executable ${CPPCON_INCLUDE_DIR}/libmysqlcppconnx-static.a)
#target_compile_options(executable PRIVATE ${CPPCON_FLAGS})