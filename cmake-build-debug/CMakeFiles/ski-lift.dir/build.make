# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\private\studies\sem6\proz\ski-lift

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\private\studies\sem6\proz\ski-lift\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\ski-lift.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\ski-lift.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\ski-lift.dir\flags.make

CMakeFiles\ski-lift.dir\main.cpp.obj: CMakeFiles\ski-lift.dir\flags.make
CMakeFiles\ski-lift.dir\main.cpp.obj: ..\main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\private\studies\sem6\proz\ski-lift\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ski-lift.dir/main.cpp.obj"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\ski-lift.dir\main.cpp.obj /FdCMakeFiles\ski-lift.dir\ /FS -c C:\private\studies\sem6\proz\ski-lift\main.cpp
<<

CMakeFiles\ski-lift.dir\main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ski-lift.dir/main.cpp.i"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe > CMakeFiles\ski-lift.dir\main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\private\studies\sem6\proz\ski-lift\main.cpp
<<

CMakeFiles\ski-lift.dir\main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ski-lift.dir/main.cpp.s"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\ski-lift.dir\main.cpp.s /c C:\private\studies\sem6\proz\ski-lift\main.cpp
<<

CMakeFiles\ski-lift.dir\QueueElement.cpp.obj: CMakeFiles\ski-lift.dir\flags.make
CMakeFiles\ski-lift.dir\QueueElement.cpp.obj: ..\QueueElement.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\private\studies\sem6\proz\ski-lift\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ski-lift.dir/QueueElement.cpp.obj"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\ski-lift.dir\QueueElement.cpp.obj /FdCMakeFiles\ski-lift.dir\ /FS -c C:\private\studies\sem6\proz\ski-lift\QueueElement.cpp
<<

CMakeFiles\ski-lift.dir\QueueElement.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ski-lift.dir/QueueElement.cpp.i"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe > CMakeFiles\ski-lift.dir\QueueElement.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\private\studies\sem6\proz\ski-lift\QueueElement.cpp
<<

CMakeFiles\ski-lift.dir\QueueElement.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ski-lift.dir/QueueElement.cpp.s"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\ski-lift.dir\QueueElement.cpp.s /c C:\private\studies\sem6\proz\ski-lift\QueueElement.cpp
<<

CMakeFiles\ski-lift.dir\ThreadState.cpp.obj: CMakeFiles\ski-lift.dir\flags.make
CMakeFiles\ski-lift.dir\ThreadState.cpp.obj: ..\ThreadState.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\private\studies\sem6\proz\ski-lift\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ski-lift.dir/ThreadState.cpp.obj"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\ski-lift.dir\ThreadState.cpp.obj /FdCMakeFiles\ski-lift.dir\ /FS -c C:\private\studies\sem6\proz\ski-lift\ThreadState.cpp
<<

CMakeFiles\ski-lift.dir\ThreadState.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ski-lift.dir/ThreadState.cpp.i"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe > CMakeFiles\ski-lift.dir\ThreadState.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\private\studies\sem6\proz\ski-lift\ThreadState.cpp
<<

CMakeFiles\ski-lift.dir\ThreadState.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ski-lift.dir/ThreadState.cpp.s"
	C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\ski-lift.dir\ThreadState.cpp.s /c C:\private\studies\sem6\proz\ski-lift\ThreadState.cpp
<<

# Object files for target ski-lift
ski__lift_OBJECTS = \
"CMakeFiles\ski-lift.dir\main.cpp.obj" \
"CMakeFiles\ski-lift.dir\QueueElement.cpp.obj" \
"CMakeFiles\ski-lift.dir\ThreadState.cpp.obj"

# External object files for target ski-lift
ski__lift_EXTERNAL_OBJECTS =

ski-lift.exe: CMakeFiles\ski-lift.dir\main.cpp.obj
ski-lift.exe: CMakeFiles\ski-lift.dir\QueueElement.cpp.obj
ski-lift.exe: CMakeFiles\ski-lift.dir\ThreadState.cpp.obj
ski-lift.exe: CMakeFiles\ski-lift.dir\build.make
ski-lift.exe: CMakeFiles\ski-lift.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\private\studies\sem6\proz\ski-lift\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ski-lift.exe"
	"C:\Program Files\JetBrains\CLion 2018.3.4\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\ski-lift.dir --manifests  -- C:\PROGRA~2\MICROS~1\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\ski-lift.dir\objects1.rsp @<<
 /out:ski-lift.exe /implib:ski-lift.lib /pdb:C:\private\studies\sem6\proz\ski-lift\cmake-build-debug\ski-lift.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\ski-lift.dir\build: ski-lift.exe

.PHONY : CMakeFiles\ski-lift.dir\build

CMakeFiles\ski-lift.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ski-lift.dir\cmake_clean.cmake
.PHONY : CMakeFiles\ski-lift.dir\clean

CMakeFiles\ski-lift.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\private\studies\sem6\proz\ski-lift C:\private\studies\sem6\proz\ski-lift C:\private\studies\sem6\proz\ski-lift\cmake-build-debug C:\private\studies\sem6\proz\ski-lift\cmake-build-debug C:\private\studies\sem6\proz\ski-lift\cmake-build-debug\CMakeFiles\ski-lift.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\ski-lift.dir\depend

