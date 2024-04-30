cmake_minimum_required(VERSION 3.27.1)
project(
    carl
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile_commands.json for LSPs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include all source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include all headers
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

