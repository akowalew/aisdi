cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(aisdi CXX)

option(AISDI_BUILD_LINEAR "Build 'Linear' subproject" ON)
option(AISDI_BUILD_MAPS "Build 'Maps' subproject" ON)

enable_testing()

add_subdirectory(submodules)

if(AISDI_BUILD_LINEAR)
	add_subdirectory(linear)
endif()

if(AISDI_BUILD_MAPS)
	add_subdirectory(maps)
endif()
