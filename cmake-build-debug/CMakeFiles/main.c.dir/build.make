# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /home/chencan/下载/clion-2023.3/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/chencan/下载/clion-2023.3/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chencan/桌面/chttpd

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chencan/桌面/chttpd/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/main.c.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main.c.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main.c.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.c.dir/flags.make

CMakeFiles/main.c.dir/main.c.o: CMakeFiles/main.c.dir/flags.make
CMakeFiles/main.c.dir/main.c.o: /home/chencan/桌面/chttpd/main.c
CMakeFiles/main.c.dir/main.c.o: CMakeFiles/main.c.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/chencan/桌面/chttpd/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main.c.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.c.dir/main.c.o -MF CMakeFiles/main.c.dir/main.c.o.d -o CMakeFiles/main.c.dir/main.c.o -c /home/chencan/桌面/chttpd/main.c

CMakeFiles/main.c.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/main.c.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/chencan/桌面/chttpd/main.c > CMakeFiles/main.c.dir/main.c.i

CMakeFiles/main.c.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/main.c.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/chencan/桌面/chttpd/main.c -o CMakeFiles/main.c.dir/main.c.s

# Object files for target main.c
main_c_OBJECTS = \
"CMakeFiles/main.c.dir/main.c.o"

# External object files for target main.c
main_c_EXTERNAL_OBJECTS =

main.c: CMakeFiles/main.c.dir/main.c.o
main.c: CMakeFiles/main.c.dir/build.make
main.c: CMakeFiles/main.c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/chencan/桌面/chttpd/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable main.c"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.c.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.c.dir/build: main.c
.PHONY : CMakeFiles/main.c.dir/build

CMakeFiles/main.c.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.c.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.c.dir/clean

CMakeFiles/main.c.dir/depend:
	cd /home/chencan/桌面/chttpd/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chencan/桌面/chttpd /home/chencan/桌面/chttpd /home/chencan/桌面/chttpd/cmake-build-debug /home/chencan/桌面/chttpd/cmake-build-debug /home/chencan/桌面/chttpd/cmake-build-debug/CMakeFiles/main.c.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/main.c.dir/depend

