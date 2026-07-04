cmake_minimum_required (VERSION 3.8)

project(VDESDemo)

if (MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:10485760")
endif()

include_directories(../Headers)

add_definitions(-DSPDLOG_COMPILED_LIB)

add_executable(VDESDemo VDESDemo.cpp sqlite3.c)

target_link_libraries(VDESDemo VDES)
