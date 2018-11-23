find_package(Boost REQUIRED)

# Unit tests

function(addUnitTest NAME)
	add_executable(${NAME}
		${NAME}.cpp
	)

	add_test(NAME ${NAME} 
		COMMAND ${NAME})

	target_include_directories(${NAME}
		PRIVATE
			${Boost_INCLUDE_DIRS}
	)

	target_link_libraries(${NAME}
		PRIVATE
			aisdi_linear
			boost_unit_test_framework
	)

	target_compile_definitions(${NAME}
		PRIVATE
			BOOST_TEST_DYN_LINK=1
	)

	target_compile_options(${NAME}
		PRIVATE
			-Wall
			-Wextra
			-pedantic
	)
endfunction()

addUnitTest(VectorTest)
addUnitTest(VectorTest2)
addUnitTest(ListTest)
addUnitTest(ListTest2)

# Benchmarks

function(addBenchmark NAME)
	add_executable(${NAME}
		${NAME}.cpp
	)

	target_link_libraries(${NAME}
		PRIVATE
			aisdi_linear
			hayai_main
	)

	target_compile_options(${NAME}
		PRIVATE
			-Wall
			-Wextra
			-pedantic
	)
endfunction()

addBenchmark(VectorBenchmark)
addBenchmark(ListBenchmark)
