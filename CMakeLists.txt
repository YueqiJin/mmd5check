# set the project name 
cmake_minimum_required(VERSION 3.4)

project(mmd5check VERSION 1.0.0 LANGUAGES CXX) 

# include dependence
include_directories (
	${PROJECT_SOURCE_DIR}/include
)

find_package(Boost 1.66.0 REQUIRED)

# build cryptopp
set(CRYPTOPP_SOURCES ${PROJECT_SOURCE_DIR}/include/cryptopp)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/cryptopp-cmake)

# specify the C++ standard

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++2a -std=c++20" COMPILER_SUPPORTS_CXX20)
if (COMPILER_SUPPORTS_CXX20)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -std=c++20 -O3")
else ()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++20 support. Please use a different C++ compiler.")
endif ()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True) 
set(CMAKE_BUILD_DIR ${PROJECT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)

# add the executable 
add_executable(mmd5check 
	src/mmd5check.cpp
)

target_link_libraries(mmd5check
	${Boost_LIBRARIES}
	cryptopp::cryptopp
)