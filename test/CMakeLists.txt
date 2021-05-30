aux_source_directory(. SRC_LIST1)
set(CMAKE_CXX_STANDARD 11)
set(LINK_SOURCE ${PROJECT_SOURCE_DIR}/src/include/hashset.h ${PROJECT_SOURCE_DIR}/src/fasta_reader.cpp ${PROJECT_SOURCE_DIR}/src/bio_string.cpp)
add_executable(test_hash_set ${SRC_LIST1} ${LINK_SOURCE})
