cmake_minimum_required(VERSION 3.10)  # CMake version check
project(DataToJson)                    # Create project
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard
set(CMAKE_CXX_COMPILER g++)           # Force g++

INCLUDE_DIRECTORIES(../Libraries/)

set(SOURCE_FILES  "DataToJson.cpp" 
	"./../Libraries/Pipeline.cpp" 
	"./../Libraries/Pipeline.h"
	"./../Libraries/ExploreDirectory.cpp"
	"./../Libraries/ExploreDirectory.h"
	"./../Libraries/FileManip.cpp"
	"./../Libraries/FileManip.h"
	)

find_package(Boost 1.65 QUIET REQUIRED COMPONENTS system filesystem)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost Not found")
endif()
add_executable("${PROJECT_NAME}" ${SOURCE_FILES})
target_link_libraries("${PROJECT_NAME}" boost_system boost_filesystem)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(EXECUTABLE_OUTPUT_PATH ./../Executables)