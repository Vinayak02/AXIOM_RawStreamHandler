cmake_minimum_required(VERSION 3.1)

project(stream_handler)

set(CMAKE_CXX_FLAGS "-pthread")

file(GLOB SOURCES_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB SOURCES_H "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(StreamHandler ${SOURCES_H} ${SOURCES_CPP})