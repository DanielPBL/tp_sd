# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/daniel/Documentos/SD/tp_sd/googletest-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/daniel/Documentos/SD/tp_sd/googletest-master

# Include any dependencies generated for this target.
include googlemock/CMakeFiles/gmock-cardinalities_test.dir/depend.make

# Include the progress variables for this target.
include googlemock/CMakeFiles/gmock-cardinalities_test.dir/progress.make

# Include the compile flags for this target's objects.
include googlemock/CMakeFiles/gmock-cardinalities_test.dir/flags.make

googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o: googlemock/CMakeFiles/gmock-cardinalities_test.dir/flags.make
googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o: googlemock/test/gmock-cardinalities_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/Documentos/SD/tp_sd/googletest-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o -c /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/test/gmock-cardinalities_test.cc

googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.i"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/test/gmock-cardinalities_test.cc > CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.i

googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.s"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/test/gmock-cardinalities_test.cc -o CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.s

googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o.requires:

.PHONY : googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o.requires

googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o.provides: googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o.requires
	$(MAKE) -f googlemock/CMakeFiles/gmock-cardinalities_test.dir/build.make googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o.provides.build
.PHONY : googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o.provides

googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o.provides.build: googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o


# Object files for target gmock-cardinalities_test
gmock__cardinalities_test_OBJECTS = \
"CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o"

# External object files for target gmock-cardinalities_test
gmock__cardinalities_test_EXTERNAL_OBJECTS =

googlemock/gmock-cardinalities_test: googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o
googlemock/gmock-cardinalities_test: googlemock/CMakeFiles/gmock-cardinalities_test.dir/build.make
googlemock/gmock-cardinalities_test: googlemock/libgmock_main.so
googlemock/gmock-cardinalities_test: googlemock/CMakeFiles/gmock-cardinalities_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/daniel/Documentos/SD/tp_sd/googletest-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable gmock-cardinalities_test"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gmock-cardinalities_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
googlemock/CMakeFiles/gmock-cardinalities_test.dir/build: googlemock/gmock-cardinalities_test

.PHONY : googlemock/CMakeFiles/gmock-cardinalities_test.dir/build

googlemock/CMakeFiles/gmock-cardinalities_test.dir/requires: googlemock/CMakeFiles/gmock-cardinalities_test.dir/test/gmock-cardinalities_test.cc.o.requires

.PHONY : googlemock/CMakeFiles/gmock-cardinalities_test.dir/requires

googlemock/CMakeFiles/gmock-cardinalities_test.dir/clean:
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock-cardinalities_test.dir/cmake_clean.cmake
.PHONY : googlemock/CMakeFiles/gmock-cardinalities_test.dir/clean

googlemock/CMakeFiles/gmock-cardinalities_test.dir/depend:
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/daniel/Documentos/SD/tp_sd/googletest-master /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock /home/daniel/Documentos/SD/tp_sd/googletest-master /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/CMakeFiles/gmock-cardinalities_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : googlemock/CMakeFiles/gmock-cardinalities_test.dir/depend

