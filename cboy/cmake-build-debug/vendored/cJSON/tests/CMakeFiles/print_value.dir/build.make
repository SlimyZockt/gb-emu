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
include vendored/cJSON/tests/CMakeFiles/print_value.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include vendored/cJSON/tests/CMakeFiles/print_value.dir/compiler_depend.make

# Include the progress variables for this target.
include vendored/cJSON/tests/CMakeFiles/print_value.dir/progress.make

# Include the compile flags for this target's objects.
include vendored/cJSON/tests/CMakeFiles/print_value.dir/flags.make

vendored/cJSON/tests/CMakeFiles/print_value.dir/codegen:
.PHONY : vendored/cJSON/tests/CMakeFiles/print_value.dir/codegen

vendored/cJSON/tests/CMakeFiles/print_value.dir/print_value.c.o: vendored/cJSON/tests/CMakeFiles/print_value.dir/flags.make
vendored/cJSON/tests/CMakeFiles/print_value.dir/print_value.c.o: /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests/print_value.c
vendored/cJSON/tests/CMakeFiles/print_value.dir/print_value.c.o: vendored/cJSON/tests/CMakeFiles/print_value.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object vendored/cJSON/tests/CMakeFiles/print_value.dir/print_value.c.o"
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT vendored/cJSON/tests/CMakeFiles/print_value.dir/print_value.c.o -MF CMakeFiles/print_value.dir/print_value.c.o.d -o CMakeFiles/print_value.dir/print_value.c.o -c /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests/print_value.c

vendored/cJSON/tests/CMakeFiles/print_value.dir/print_value.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/print_value.dir/print_value.c.i"
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests/print_value.c > CMakeFiles/print_value.dir/print_value.c.i

vendored/cJSON/tests/CMakeFiles/print_value.dir/print_value.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/print_value.dir/print_value.c.s"
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests/print_value.c -o CMakeFiles/print_value.dir/print_value.c.s

# Object files for target print_value
print_value_OBJECTS = \
"CMakeFiles/print_value.dir/print_value.c.o"

# External object files for target print_value
print_value_EXTERNAL_OBJECTS =

Debug/print_value: vendored/cJSON/tests/CMakeFiles/print_value.dir/print_value.c.o
Debug/print_value: vendored/cJSON/tests/CMakeFiles/print_value.dir/build.make
Debug/print_value: vendored/cJSON/tests/CMakeFiles/print_value.dir/compiler_depend.ts
Debug/print_value: Debug/libcjson.so.1.7.18
Debug/print_value: vendored/cJSON/tests/libunity.a
Debug/print_value: vendored/cJSON/tests/CMakeFiles/print_value.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../../../Debug/print_value"
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/print_value.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
vendored/cJSON/tests/CMakeFiles/print_value.dir/build: Debug/print_value
.PHONY : vendored/cJSON/tests/CMakeFiles/print_value.dir/build

vendored/cJSON/tests/CMakeFiles/print_value.dir/clean:
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests && $(CMAKE_COMMAND) -P CMakeFiles/print_value.dir/cmake_clean.cmake
.PHONY : vendored/cJSON/tests/CMakeFiles/print_value.dir/clean

vendored/cJSON/tests/CMakeFiles/print_value.dir/depend:
	cd /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/myt/Dokumente/projects/gb-emu/cboy /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/tests /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests /home/myt/Dokumente/projects/gb-emu/cboy/cmake-build-debug/vendored/cJSON/tests/CMakeFiles/print_value.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : vendored/cJSON/tests/CMakeFiles/print_value.dir/depend

