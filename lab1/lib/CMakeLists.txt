add_library(aisdi_lab1
	src/Car.cpp
)

target_include_directories(aisdi_lab1 
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)
