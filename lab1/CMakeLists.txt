add_library(aisdi_lab1
	src/Car.cpp
)

set_property(TARGET aisdi_lab1 PROPERTY CXX_STANDARD 11)

target_include_directories(aisdi_lab1 
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(test)