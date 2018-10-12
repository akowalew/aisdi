add_executable(Car-test 
	Car-test.cpp
)

target_compile_features(Car-test
	PRIVATE
		cxx_auto_type
)

target_link_libraries(Car-test
		aisdi_lab1
		Catch2::Catch2
)

add_test(NAME Car-test COMMAND Car-test)