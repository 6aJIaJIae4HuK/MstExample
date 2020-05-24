cmake_minimum_required(VERSION 3.0.2)

project(MstExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
file(GLOB SRCS "*.cpp")
add_executable(MstExamples ${SRCS})
target_link_libraries(MstExamples ${PNG_LIBRARY})
target_link_libraries(MstExamples stdc++fs)
