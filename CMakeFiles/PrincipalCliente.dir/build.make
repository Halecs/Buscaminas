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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas

# Include any dependencies generated for this target.
include CMakeFiles/PrincipalCliente.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/PrincipalCliente.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PrincipalCliente.dir/flags.make

CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o: CMakeFiles/PrincipalCliente.dir/flags.make
CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o: PrincipalCliente.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o -c /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas/PrincipalCliente.cpp

CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas/PrincipalCliente.cpp > CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.i

CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas/PrincipalCliente.cpp -o CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.s

CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o.requires:
.PHONY : CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o.requires

CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o.provides: CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o.requires
	$(MAKE) -f CMakeFiles/PrincipalCliente.dir/build.make CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o.provides.build
.PHONY : CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o.provides

CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o.provides.build: CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o

# Object files for target PrincipalCliente
PrincipalCliente_OBJECTS = \
"CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o"

# External object files for target PrincipalCliente
PrincipalCliente_EXTERNAL_OBJECTS =

PrincipalCliente: CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o
PrincipalCliente: CMakeFiles/PrincipalCliente.dir/build.make
PrincipalCliente: CMakeFiles/PrincipalCliente.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable PrincipalCliente"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PrincipalCliente.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PrincipalCliente.dir/build: PrincipalCliente
.PHONY : CMakeFiles/PrincipalCliente.dir/build

CMakeFiles/PrincipalCliente.dir/requires: CMakeFiles/PrincipalCliente.dir/PrincipalCliente.cpp.o.requires
.PHONY : CMakeFiles/PrincipalCliente.dir/requires

CMakeFiles/PrincipalCliente.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PrincipalCliente.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PrincipalCliente.dir/clean

CMakeFiles/PrincipalCliente.dir/depend:
	cd /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas /mnt/c/Users/Halecs/Desktop/Buscaminas/Buscaminas/CMakeFiles/PrincipalCliente.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PrincipalCliente.dir/depend
