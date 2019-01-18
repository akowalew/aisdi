cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(aisdi_graphs CXX)

option(AISDI_GRAPHS_BUILD_TESTS "Build tests for project" ON)

add_library(aisdi_graphs INTERFACE)

if(CMAKE_BUILD_TYPE MATCHES Release)
	message(STATUS "Adding GSL_UNENFORCED_ON_CONTRACT_VIOLATION definition...")
	target_compile_definitions(aisdi_graphs
		INTERFACE
			GSL_UNENFORCED_ON_CONTRACT_VIOLATION
	)
elseif(CMAKE_BUILD_TYPE )
	message(STATUS "Adding GSL_THROW_ON_CONTRACT_VIOLATION definition...")
	target_compile_definitions(aisdi_graphs
		INTERFACE
			GSL_THROW_ON_CONTRACT_VIOLATION
	)
endif()

target_include_directories(aisdi_graphs
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/../maps/include
)

target_link_libraries(aisdi_graphs
	INTERFACE
		GSL
		aisdi_maps
)

if(MSVC)
	target_compile_options(aisdi_graphs
		INTERFACE
			/std:c++14
	)
else(MSVC)
	target_compile_options(aisdi_graphs
		INTERFACE
			-std=c++14
	)
endif(MSVC)

# Compile library to check for basic compiler errors
add_library(aisdi_graphs_compiled
	src/Graph.cpp
)

target_link_libraries(aisdi_graphs_compiled
	PRIVATE
		aisdi_graphs
)

if(MSVC)
	target_compile_options(aisdi_graphs_compiled
		INTERFACE
			/W4 /WX
	)
else(MSVC)
	target_compile_options(aisdi_graphs_compiled
		INTERFACE
			-Wall
			-Wextra
			-pedantic
			-Wcast-align
			-Wunused
			-Wpedantic
			-Wconversion
			-Wsign-conversion
			-Wnull-dereference
	)
endif(MSVC)

# Add tests to this project
if(AISDI_GRAPHS_BUILD_TESTS)
	add_subdirectory(test)
endif()
