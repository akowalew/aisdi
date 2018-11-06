find_package(Boost REQUIRED)

add_executable(VectorTest
	VectorTest.cpp
)

add_test(NAME VectorTest COMMAND VectorTest)

target_include_directories(VectorTest
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(VectorTest
	PRIVATE
		aisdi_linear
		boost_unit_test_framework
)

target_compile_definitions(VectorTest 
	PRIVATE 
		BOOST_TEST_DYN_LINK=1
)

add_executable(ListTest
	ListTest.cpp
)

add_test(NAME ListTest COMMAND ListTest)

target_include_directories(ListTest
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(ListTest
	PRIVATE
		aisdi_linear
		boost_unit_test_framework
)

target_compile_definitions(ListTest 
	PRIVATE 
		BOOST_TEST_DYN_LINK=1
)