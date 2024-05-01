cmake_minimum_required(VERSION 3.27.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.0
)

FetchContent_MakeAvailable(Catch2)

# Include all unit tests and source files (except main.cpp) for project
file(GLOB_RECURSE SRC_FILES ../src/*.cpp)
file(GLOB_RECURSE TEST_SRC_FILES *.cpp)
add_executable(tests ${SRC_FILES} ${TEST_SRC_FILES})

# Include all headers
target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
