cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(DelayedConstructor LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) # !IsSubproject()
    find_package(GTest)
    if(GTEST_FOUND)
        enable_testing()
    endif()
endif()

if(NOT TARGET DelayedConstructor)
    add_library(DelayedConstructor INTERFACE)
    target_include_directories(DelayedConstructor INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
