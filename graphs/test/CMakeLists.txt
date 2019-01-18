find_package(Boost REQUIRED)

add_executable(aisdi_graphs_tests
	tests_main.cpp

	GraphTest.cpp
	graph_io_Test.cpp
	distant_bridges_Test.cpp
	connected_components_count_Test.cpp
)

target_include_directories(aisdi_graphs_tests
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(aisdi_graphs_tests
	PRIVATE
		aisdi_graphs
		boost_unit_test_framework
)

target_compile_definitions(aisdi_graphs_tests
	PRIVATE
		BOOST_TEST_DYN_LINK=1
)

target_compile_options(aisdi_graphs_tests
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

add_test(GraphTest aisdi_graphs_tests --run_test=GraphTest)
add_test(graph_io_Test aisdi_graphs_tests --run_test=graph_io_Test)
add_test(distant_bridges_Test aisdi_graphs_tests --run_test=distant_bridges_Test)
add_test(connected_components_count_Test aisdi_graphs_tests --run_test=connected_components_count_Test)
