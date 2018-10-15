add_executable(lab1_tests
	car-test.cpp
	segfault-test.cpp
	memleak-test.cpp

	tests-main.cpp
)

target_link_libraries(lab1_tests
	PRIVATE
		lab1
		Catch2::Catch2
)

target_compile_options(lab1_tests
	PRIVATE
		-std=c++17
)

add_test(NAME lab1_tests COMMAND lab1_tests)
