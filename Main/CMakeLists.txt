cmake_minimum_required(VERSION 3.10)  # CMake version check
project(CFurcatus)                    # Create project
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard
set(CMAKE_CXX_COMPILER g++)           # Force g++
set(CMAKE_VERBOSE_MAKEFILE ON)        # Verbose

INCLUDE_DIRECTORIES(../Libraries/)

set(SOURCE_FILES  "Main.cpp" 
	"./../Libraries/Pipeline.cpp" 
	"./../Libraries/ExploreDirectory.cpp"
	"./../Libraries/FileManip.cpp"
	"./../Libraries/Utility.cpp"
	"DataToJson.cpp"
	"CartesianToSpherical.cpp"
	"Variations.cpp"
	"MixData.cpp"
	"DivideData.cpp"
	"RunningAverage.cpp"
	"Displacement.cpp"
	"DivideFastSlow.cpp"
	)

find_package(Boost 1.65 QUIET REQUIRED COMPONENTS system filesystem)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost Not found")
endif()
add_executable("${PROJECT_NAME}" ${SOURCE_FILES})
target_link_libraries("${PROJECT_NAME}" boost_system boost_filesystem)

if(NOT CMAKE_BUILD_TYPE)
	#set(CMAKE_BUILD_TYPE Debug)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Wpedantic -fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(EXECUTABLE_OUTPUT_PATH ./../Executables)