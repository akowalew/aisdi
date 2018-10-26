find_package(Boost REQUIRED)

add_executable(aisdi_linear_tests
	tests_main.cpp

	ListTests.cpp
	VectorTests.cpp
)

add_test(NAME aisdi_linear_tests COMMAND aisdi_linear_tests)

target_include_directories(aisdi_linear_tests
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(aisdi_linear_tests
	PRIVATE
		aisdi_linear
		boost_unit_test_framework
)

target_compile_definitions(aisdi_linear_tests 
	PRIVATE 
		BOOST_TEST_DYN_LINK=1
)