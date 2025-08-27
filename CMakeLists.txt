cmake_minimum_required(VERSION 3.10)
project(SunWorld CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_compile_options(SunWorld PRIVATE -Wall -Wextra -O2 -Iinclude)

set(RAYLIB ${CMAKE_SOURCE_DIR}/libs/libraylib.a)

target_link_libraries(SunWorld PRIVATE
    ${RAYLIB}
    opengl32
    gdi32
    winmm
    m
)

option(RUN_AFTER_BUILD "Run after building" ON)

if(RUN_AFTER_BUILD)
    add_custom_target(run_${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E echo "Running ${PROJECT_NAME}..."
        COMMAND ${CMAKE_COMMAND} -E env $<TARGET_FILE:${PROJECT_NAME}>
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()