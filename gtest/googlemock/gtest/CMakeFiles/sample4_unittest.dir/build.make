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
include googlemock/gtest/CMakeFiles/sample4_unittest.dir/depend.make

# Include the progress variables for this target.
include googlemock/gtest/CMakeFiles/sample4_unittest.dir/progress.make

# Include the compile flags for this target's objects.
include googlemock/gtest/CMakeFiles/sample4_unittest.dir/flags.make

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o: googlemock/gtest/CMakeFiles/sample4_unittest.dir/flags.make
googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o: googletest/samples/sample4_unittest.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/Documentos/SD/tp_sd/googletest-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o -c /home/daniel/Documentos/SD/tp_sd/googletest-master/googletest/samples/sample4_unittest.cc

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.i"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/Documentos/SD/tp_sd/googletest-master/googletest/samples/sample4_unittest.cc > CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.i

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.s"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/Documentos/SD/tp_sd/googletest-master/googletest/samples/sample4_unittest.cc -o CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.s

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o.requires:

.PHONY : googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o.requires

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o.provides: googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o.requires
	$(MAKE) -f googlemock/gtest/CMakeFiles/sample4_unittest.dir/build.make googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o.provides.build
.PHONY : googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o.provides

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o.provides.build: googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o


googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o: googlemock/gtest/CMakeFiles/sample4_unittest.dir/flags.make
googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o: googletest/samples/sample4.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/Documentos/SD/tp_sd/googletest-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o -c /home/daniel/Documentos/SD/tp_sd/googletest-master/googletest/samples/sample4.cc

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sample4_unittest.dir/samples/sample4.cc.i"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/Documentos/SD/tp_sd/googletest-master/googletest/samples/sample4.cc > CMakeFiles/sample4_unittest.dir/samples/sample4.cc.i

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sample4_unittest.dir/samples/sample4.cc.s"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/Documentos/SD/tp_sd/googletest-master/googletest/samples/sample4.cc -o CMakeFiles/sample4_unittest.dir/samples/sample4.cc.s

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o.requires:

.PHONY : googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o.requires

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o.provides: googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o.requires
	$(MAKE) -f googlemock/gtest/CMakeFiles/sample4_unittest.dir/build.make googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o.provides.build
.PHONY : googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o.provides

googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o.provides.build: googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o


# Object files for target sample4_unittest
sample4_unittest_OBJECTS = \
"CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o" \
"CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o"

# External object files for target sample4_unittest
sample4_unittest_EXTERNAL_OBJECTS =

googlemock/gtest/sample4_unittest: googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o
googlemock/gtest/sample4_unittest: googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o
googlemock/gtest/sample4_unittest: googlemock/gtest/CMakeFiles/sample4_unittest.dir/build.make
googlemock/gtest/sample4_unittest: googlemock/gtest/libgtest_main.so
googlemock/gtest/sample4_unittest: googlemock/gtest/libgtest.so
googlemock/gtest/sample4_unittest: googlemock/gtest/CMakeFiles/sample4_unittest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/daniel/Documentos/SD/tp_sd/googletest-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable sample4_unittest"
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sample4_unittest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
googlemock/gtest/CMakeFiles/sample4_unittest.dir/build: googlemock/gtest/sample4_unittest

.PHONY : googlemock/gtest/CMakeFiles/sample4_unittest.dir/build

googlemock/gtest/CMakeFiles/sample4_unittest.dir/requires: googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4_unittest.cc.o.requires
googlemock/gtest/CMakeFiles/sample4_unittest.dir/requires: googlemock/gtest/CMakeFiles/sample4_unittest.dir/samples/sample4.cc.o.requires

.PHONY : googlemock/gtest/CMakeFiles/sample4_unittest.dir/requires

googlemock/gtest/CMakeFiles/sample4_unittest.dir/clean:
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest && $(CMAKE_COMMAND) -P CMakeFiles/sample4_unittest.dir/cmake_clean.cmake
.PHONY : googlemock/gtest/CMakeFiles/sample4_unittest.dir/clean

googlemock/gtest/CMakeFiles/sample4_unittest.dir/depend:
	cd /home/daniel/Documentos/SD/tp_sd/googletest-master && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/daniel/Documentos/SD/tp_sd/googletest-master /home/daniel/Documentos/SD/tp_sd/googletest-master/googletest /home/daniel/Documentos/SD/tp_sd/googletest-master /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest /home/daniel/Documentos/SD/tp_sd/googletest-master/googlemock/gtest/CMakeFiles/sample4_unittest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : googlemock/gtest/CMakeFiles/sample4_unittest.dir/depend

