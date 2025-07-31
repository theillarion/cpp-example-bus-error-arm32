cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(fc_crash
    VERSION 11.00
    DESCRIPTION "ABAK+ FC Crash"
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(source
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core_dump.cpp)

add_executable(${PROJECT_NAME}_1 ${source})
target_include_directories(${PROJECT_NAME}_1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(${PROJECT_NAME}_1 PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}.${CMAKE_BUILD_TYPE}.1")

add_executable(${PROJECT_NAME}_4 ${source})
target_include_directories(${PROJECT_NAME}_4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/)
target_compile_definitions(${PROJECT_NAME}_4 PRIVATE "-DPADDING_SIZE=4")

set_target_properties(${PROJECT_NAME}_4 PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}.${CMAKE_BUILD_TYPE}.4")