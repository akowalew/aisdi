cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(aisdi_linear CXX)

option(BUILD_TESTS "Build tests for project" ON)

if(BUILD_TESTS)
	enable_testing()
endif()

add_library(aisdi_linear INTERFACE)

target_include_directories(aisdi_linear
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(aisdi_linear
	INTERFACE
		-std=c++14
		-Wall
		-Wextra
		-pedantic
)

if(BUILD_TESTS)
	add_subdirectory(submodules/hayai)
	include_directories(submodules/hayai/src)
	add_subdirectory(test)
endif()
