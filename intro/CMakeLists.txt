add_library(lab1
	src/Car.cpp
)

target_include_directories(lab1
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(test)
