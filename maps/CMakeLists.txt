cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(aisdi_maps CXX)

option(AISDI_MAPS_BUILD_TESTS "Build tests for project" ON)

add_library(aisdi_maps INTERFACE)

if(CMAKE_BUILD_TYPE MATCHES Release)
	message(STATUS "Adding GSL_UNENFORCED_ON_CONTRACT_VIOLATION definition...")
	target_compile_definitions(aisdi_maps
		INTERFACE
			GSL_UNENFORCED_ON_CONTRACT_VIOLATION
	)
elseif(CMAKE_BUILD_TYPE )
	message(STATUS "Adding GSL_THROW_ON_CONTRACT_VIOLATION definition...")
	target_compile_definitions(aisdi_maps
		INTERFACE
			GSL_THROW_ON_CONTRACT_VIOLATION
	)
endif()

target_include_directories(aisdi_maps
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/linear/include
		${CMAKE_SOURCE_DIR}/submodules/gsl/include
)

target_link_libraries(aisdi_maps
	INTERFACE
		GSL
		aisdi_linear
)

target_compile_features(aisdi_maps
	INTERFACE
		cxx_auto_type
		cxx_constexpr
		cxx_defaulted_functions
		cxx_defaulted_move_initializers
		cxx_delegating_constructors
		cxx_generalized_initializers
		cxx_generic_lambdas
		cxx_inheriting_constructors
		cxx_lambdas
		cxx_lambda_init_captures
		cxx_noexcept
		cxx_nullptr
		cxx_range_for
		cxx_return_type_deduction
		cxx_rvalue_references
		cxx_static_assert
)

add_library(aisdi_maps_compiled
	src/TreeMapCompiled.cpp
	src/HashMapCompiled.cpp
)

target_link_libraries(aisdi_maps_compiled
	PRIVATE
		aisdi_maps
)

if(AISDI_MAPS_BUILD_TESTS)
	add_subdirectory(test)
endif()
