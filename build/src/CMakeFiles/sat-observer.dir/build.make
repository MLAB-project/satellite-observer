# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/cmake-gui

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/carlos/Escritorio/mlab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/carlos/Escritorio/mlab/build

# Include any dependencies generated for this target.
include src/CMakeFiles/sat-observer.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/sat-observer.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/sat-observer.dir/flags.make

src/CMakeFiles/sat-observer.dir/main.cpp.o: src/CMakeFiles/sat-observer.dir/flags.make
src/CMakeFiles/sat-observer.dir/main.cpp.o: ../src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/carlos/Escritorio/mlab/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/sat-observer.dir/main.cpp.o"
	cd /home/carlos/Escritorio/mlab/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sat-observer.dir/main.cpp.o -c /home/carlos/Escritorio/mlab/src/main.cpp

src/CMakeFiles/sat-observer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sat-observer.dir/main.cpp.i"
	cd /home/carlos/Escritorio/mlab/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/carlos/Escritorio/mlab/src/main.cpp > CMakeFiles/sat-observer.dir/main.cpp.i

src/CMakeFiles/sat-observer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sat-observer.dir/main.cpp.s"
	cd /home/carlos/Escritorio/mlab/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/carlos/Escritorio/mlab/src/main.cpp -o CMakeFiles/sat-observer.dir/main.cpp.s

src/CMakeFiles/sat-observer.dir/main.cpp.o.requires:
.PHONY : src/CMakeFiles/sat-observer.dir/main.cpp.o.requires

src/CMakeFiles/sat-observer.dir/main.cpp.o.provides: src/CMakeFiles/sat-observer.dir/main.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/sat-observer.dir/build.make src/CMakeFiles/sat-observer.dir/main.cpp.o.provides.build
.PHONY : src/CMakeFiles/sat-observer.dir/main.cpp.o.provides

src/CMakeFiles/sat-observer.dir/main.cpp.o.provides.build: src/CMakeFiles/sat-observer.dir/main.cpp.o

# Object files for target sat-observer
sat__observer_OBJECTS = \
"CMakeFiles/sat-observer.dir/main.cpp.o"

# External object files for target sat-observer
sat__observer_EXTERNAL_OBJECTS =

src/sat-observer: src/CMakeFiles/sat-observer.dir/main.cpp.o
src/sat-observer: src/CMakeFiles/sat-observer.dir/build.make
src/sat-observer: src/others/libTLE.a
src/sat-observer: src/others/libSATELLITE.a
src/sat-observer: src/orbitTools/core/libCORE.a
src/sat-observer: src/others/libUTILITIES.a
src/sat-observer: src/CMakeFiles/sat-observer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable sat-observer"
	cd /home/carlos/Escritorio/mlab/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sat-observer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/sat-observer.dir/build: src/sat-observer
.PHONY : src/CMakeFiles/sat-observer.dir/build

src/CMakeFiles/sat-observer.dir/requires: src/CMakeFiles/sat-observer.dir/main.cpp.o.requires
.PHONY : src/CMakeFiles/sat-observer.dir/requires

src/CMakeFiles/sat-observer.dir/clean:
	cd /home/carlos/Escritorio/mlab/build/src && $(CMAKE_COMMAND) -P CMakeFiles/sat-observer.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/sat-observer.dir/clean

src/CMakeFiles/sat-observer.dir/depend:
	cd /home/carlos/Escritorio/mlab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/carlos/Escritorio/mlab /home/carlos/Escritorio/mlab/src /home/carlos/Escritorio/mlab/build /home/carlos/Escritorio/mlab/build/src /home/carlos/Escritorio/mlab/build/src/CMakeFiles/sat-observer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/sat-observer.dir/depend

