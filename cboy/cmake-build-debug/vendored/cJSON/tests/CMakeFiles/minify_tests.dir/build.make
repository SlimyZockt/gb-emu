# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/myt/Dokumente/projects/gb-emu/cboy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug

# Include any dependencies generated for this target.
include vendored/cJSON/tests/CMakeFiles/minify_tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include vendored/cJSON/tests/CMakeFiles/minify_tests.dir/compiler_depend.make

# Include the progress variables for this target.
include vendored/cJSON/tests/CMakeFiles/minify_tests.dir/progress.make

# Include the compile flags for this target's objects.
include vendored/cJSON/tests/CMakeFiles/minify_tests.dir/flags.make

vendored/cJSON/tests/CMakeFiles/minify_tests.dir/codegen:
.PHONY : vendored/cJSON/tests/CMakeFiles/minify_tests.dir/codegen

vendored/cJSON/tests/CMakeFiles/minify_tests.dir/minify_tests.c.o: vendored/cJSON/tests/CMakeFiles/minify_tests.dir/flags.make
vendored/cJSON/tests/CMakeFiles/minify_tests.dir/minify_tests.c.o: /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests/minify_tests.c
vendored/cJSON/tests/CMakeFiles/minify_tests.dir/minify_tests.c.o: vendored/cJSON/tests/CMakeFiles/minify_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object vendored/cJSON/tests/CMakeFiles/minify_tests.dir/minify_tests.c.o"
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT vendored/cJSON/tests/CMakeFiles/minify_tests.dir/minify_tests.c.o -MF CMakeFiles/minify_tests.dir/minify_tests.c.o.d -o CMakeFiles/minify_tests.dir/minify_tests.c.o -c /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests/minify_tests.c

vendored/cJSON/tests/CMakeFiles/minify_tests.dir/minify_tests.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/minify_tests.dir/minify_tests.c.i"
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests/minify_tests.c > CMakeFiles/minify_tests.dir/minify_tests.c.i

vendored/cJSON/tests/CMakeFiles/minify_tests.dir/minify_tests.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/minify_tests.dir/minify_tests.c.s"
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests/minify_tests.c -o CMakeFiles/minify_tests.dir/minify_tests.c.s

# Object files for target minify_tests
minify_tests_OBJECTS = \
"CMakeFiles/minify_tests.dir/minify_tests.c.o"

# External object files for target minify_tests
minify_tests_EXTERNAL_OBJECTS =

Debug/minify_tests: vendored/cJSON/tests/CMakeFiles/minify_tests.dir/minify_tests.c.o
Debug/minify_tests: vendored/cJSON/tests/CMakeFiles/minify_tests.dir/build.make
Debug/minify_tests: vendored/cJSON/tests/CMakeFiles/minify_tests.dir/compiler_depend.ts
Debug/minify_tests: Debug/libcjson.so.1.7.18
Debug/minify_tests: vendored/cJSON/tests/libunity.a
Debug/minify_tests: vendored/cJSON/tests/CMakeFiles/minify_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../../../Debug/minify_tests"
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/minify_tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
vendored/cJSON/tests/CMakeFiles/minify_tests.dir/build: Debug/minify_tests
.PHONY : vendored/cJSON/tests/CMakeFiles/minify_tests.dir/build

vendored/cJSON/tests/CMakeFiles/minify_tests.dir/clean:
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && $(CMAKE_COMMAND) -P CMakeFiles/minify_tests.dir/cmake_clean.cmake
.PHONY : vendored/cJSON/tests/CMakeFiles/minify_tests.dir/clean

vendored/cJSON/tests/CMakeFiles/minify_tests.dir/depend:
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/myt/Dokumente/projects/gb-emu/cboy /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests/CMakeFiles/minify_tests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : vendored/cJSON/tests/CMakeFiles/minify_tests.dir/depend

