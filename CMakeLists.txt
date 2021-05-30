cmake_minimum_required(VERSION 3.15)
project(SomaticReadsFilter)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/src/mult_thread)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart)
link_directories(${PROJECT_SOURCE_DIR}/thirdpart)

add_subdirectory(src)
add_subdirectory(test)