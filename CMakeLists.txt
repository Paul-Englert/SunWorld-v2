cmake_minimum_required(VERSION 3.10)
project(SunWorld CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

set(RAYLIB ${CMAKE_SOURCE_DIR}/libs/libraylib.a)

target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB})