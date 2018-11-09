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

add_executable(VectorTest2
	VectorTest2.cpp
)

add_test(NAME VectorTest2 COMMAND VectorTest2)

target_include_directories(VectorTest2
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(VectorTest2
	PRIVATE
		aisdi_linear
		boost_unit_test_framework
)

target_compile_definitions(VectorTest2
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

add_executable(ListTest2
	ListTest2.cpp
)

add_test(NAME ListTest2 COMMAND ListTest2)

target_include_directories(ListTest2
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(ListTest2
	PRIVATE
		aisdi_linear
		boost_unit_test_framework
)

target_compile_definitions(ListTest2
	PRIVATE
		BOOST_TEST_DYN_LINK=1
)
