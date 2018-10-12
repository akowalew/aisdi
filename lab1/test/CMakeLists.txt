# creates the executable
add_executable(Car-test 
	Car-test.cpp
)

# indicates the link paths
target_link_libraries(Car-test
	PRIVATE
		aisdi_lab1
)

# declares a test with our executable
add_test(NAME Car-test COMMAND Car-test)