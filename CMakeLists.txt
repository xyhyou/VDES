# CMakeList.txt: VDES 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required(VERSION 3.23)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  # 当启用ASAN时，使用/Zi而不是/ZI
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  endif()
endif()

project(VDES)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set configurations
set(CMAKE_CXX_STANDARD 11)

include_directories(./Headers)
include_directories(./Headers/eventpp)
include_directories(./Headers/spdlog)

set(SRC_DIR_LIST "")

file(GLOB_RECURSE SRC_DIR_LIST "./Sources/*.cpp")

add_definitions(-DSPDLOG_COMPILED_LIB)
add_definitions(-DSQLITE_ENABLE_RTREE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DENABLE_PARSE_TDB)
	#add_definitions(-DENABLE_PARSE_ABB)
endif()

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)
	add_compile_options(/utf-8)
	
	# Enable Address Sanitizer for Debug builds
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /fsanitize=address")
		# Disable incremental linking which is not compatible with ASAN
		set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL:NO")
		set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /INCREMENTAL:NO")
	endif()
endif()

add_library(VDES STATIC ${SRC_DIR_LIST})

# TODO: 如有需要，请添加测试并安装目标。
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_subdirectory("VDESDemo")
endif()
