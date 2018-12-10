find_package(Boost REQUIRED)

add_executable(aisdi_maps_tests
	tests_main.cpp

	TreeMapTests.cpp
	HashMapTests.cpp
)

target_include_directories(aisdi_maps_tests
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(aisdi_maps_tests
	PRIVATE
		aisdi_maps
		boost_unit_test_framework
)

target_compile_definitions(aisdi_maps_tests
	PRIVATE
		BOOST_TEST_DYN_LINK=1
)

target_compile_options(aisdi_maps_tests
	PRIVATE
		-Wall
		-Wextra
		-pedantic
)

add_test(TreeMapTests aisdi_maps_tests --run_test=TreeMapTests)
add_test(HashMapTests aisdi_maps_tests --run_test=HashMapTests)

