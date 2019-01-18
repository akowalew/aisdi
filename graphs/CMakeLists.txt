cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(aisdi_graphs CXX)

option(AISDI_GRAPHS_BUILD_TESTS "Build tests for project" ON)

add_library(aisdi_graphs
	src/Graph.cpp include/aisdi/Graph.hpp
	src/graph_io.cpp include/aisdi/graph_io.hpp
	include/aisdi/distant_bridges.hpp
	src/connected_components_count.cpp include/aisdi/connected_components_count.hpp
)

add_executable(distant_bridges
	src/distant_bridges.cpp)
target_link_libraries(distant_bridges
	aisdi_graphs)

if(CMAKE_BUILD_TYPE MATCHES Release)
	message(STATUS "Adding GSL_UNENFORCED_ON_CONTRACT_VIOLATION definition...")
	target_compile_definitions(aisdi_graphs
		PUBLIC
			GSL_UNENFORCED_ON_CONTRACT_VIOLATION
	)
elseif(CMAKE_BUILD_TYPE )
	message(STATUS "Adding GSL_THROW_ON_CONTRACT_VIOLATION definition...")
	target_compile_definitions(aisdi_graphs
		PUBLIC
			GSL_THROW_ON_CONTRACT_VIOLATION
	)
endif()

target_include_directories(aisdi_graphs
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/../maps/include
)

target_link_libraries(aisdi_graphs
	PUBLIC
		GSL
		aisdi_maps
)

if(MSVC)
	target_compile_options(aisdi_graphs
		PUBLIC
			/std:c++14
		PRIVATE
			/W4 /WX
	)
else(MSVC)
	target_compile_options(aisdi_graphs
		PUBLIC
			-std=c++14
		PRIVATE
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
