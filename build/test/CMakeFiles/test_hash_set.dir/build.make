# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/cmake/bin/cmake

# The command to remove a file.
RM = /usr/local/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/SomaticReadsFilter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/SomaticReadsFilter/build

# Include any dependencies generated for this target.
include test/CMakeFiles/test_hash_set.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_hash_set.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_hash_set.dir/flags.make

test/CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.o: test/CMakeFiles/test_hash_set.dir/flags.make
test/CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.o: ../test/test_create_hashset.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/SomaticReadsFilter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.o"
	cd /root/SomaticReadsFilter/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.o -c /root/SomaticReadsFilter/test/test_create_hashset.cpp

test/CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.i"
	cd /root/SomaticReadsFilter/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/SomaticReadsFilter/test/test_create_hashset.cpp > CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.i

test/CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.s"
	cd /root/SomaticReadsFilter/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/SomaticReadsFilter/test/test_create_hashset.cpp -o CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.s

test/CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.o: test/CMakeFiles/test_hash_set.dir/flags.make
test/CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.o: ../src/fasta_reader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/SomaticReadsFilter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.o"
	cd /root/SomaticReadsFilter/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.o -c /root/SomaticReadsFilter/src/fasta_reader.cpp

test/CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.i"
	cd /root/SomaticReadsFilter/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/SomaticReadsFilter/src/fasta_reader.cpp > CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.i

test/CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.s"
	cd /root/SomaticReadsFilter/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/SomaticReadsFilter/src/fasta_reader.cpp -o CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.s

test/CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.o: test/CMakeFiles/test_hash_set.dir/flags.make
test/CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.o: ../src/bio_string.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/SomaticReadsFilter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object test/CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.o"
	cd /root/SomaticReadsFilter/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.o -c /root/SomaticReadsFilter/src/bio_string.cpp

test/CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.i"
	cd /root/SomaticReadsFilter/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/SomaticReadsFilter/src/bio_string.cpp > CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.i

test/CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.s"
	cd /root/SomaticReadsFilter/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/SomaticReadsFilter/src/bio_string.cpp -o CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.s

# Object files for target test_hash_set
test_hash_set_OBJECTS = \
"CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.o" \
"CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.o" \
"CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.o"

# External object files for target test_hash_set
test_hash_set_EXTERNAL_OBJECTS =

../bin/test_hash_set: test/CMakeFiles/test_hash_set.dir/test_create_hashset.cpp.o
../bin/test_hash_set: test/CMakeFiles/test_hash_set.dir/__/src/fasta_reader.cpp.o
../bin/test_hash_set: test/CMakeFiles/test_hash_set.dir/__/src/bio_string.cpp.o
../bin/test_hash_set: test/CMakeFiles/test_hash_set.dir/build.make
../bin/test_hash_set: test/CMakeFiles/test_hash_set.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/SomaticReadsFilter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../bin/test_hash_set"
	cd /root/SomaticReadsFilter/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_hash_set.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_hash_set.dir/build: ../bin/test_hash_set

.PHONY : test/CMakeFiles/test_hash_set.dir/build

test/CMakeFiles/test_hash_set.dir/clean:
	cd /root/SomaticReadsFilter/build/test && $(CMAKE_COMMAND) -P CMakeFiles/test_hash_set.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_hash_set.dir/clean

test/CMakeFiles/test_hash_set.dir/depend:
	cd /root/SomaticReadsFilter/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/SomaticReadsFilter /root/SomaticReadsFilter/test /root/SomaticReadsFilter/build /root/SomaticReadsFilter/build/test /root/SomaticReadsFilter/build/test/CMakeFiles/test_hash_set.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_hash_set.dir/depend

