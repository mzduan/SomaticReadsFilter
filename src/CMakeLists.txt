aux_source_directory(. SRC_LIST)
set(CMAKE_CXX_STANDARD 11)
add_executable(SomaticReadsFilter ${SRC_LIST})
target_link_libraries(SomaticReadsFilter m bam z)