cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(aisdi_linear CXX)

option(AISDI_LINEAR_BUILD_TESTS "Build tests for project" ON)

enable_testing()

add_library(aisdi_linear INTERFACE)

if(CMAKE_BUILD_TYPE MATCHES Release)
	message(STATUS "Adding GSL_UNENFORCED_ON_CONTRACT_VIOLATION definition...")
	target_compile_definitions(aisdi_linear
		INTERFACE
			GSL_UNENFORCED_ON_CONTRACT_VIOLATION
	)
elseif(CMAKE_BUILD_TYPE )
	message(STATUS "Adding GSL_THROW_ON_CONTRACT_VIOLATION definition...")
	target_compile_definitions(aisdi_linear
		INTERFACE
			GSL_THROW_ON_CONTRACT_VIOLATION
	)
endif()

target_include_directories(aisdi_linear
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(aisdi_linear
	INTERFACE
		GSL
)

target_compile_features(aisdi_linear
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

if(AISDI_LINEAR_BUILD_TESTS)
	add_subdirectory(test)
endif()
