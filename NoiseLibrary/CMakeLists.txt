cmake_minimum_required(VERSION 3.20.0)
project(NoiseLibrary VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
add_subdirectory(dependencies/FastNoise2)
add_subdirectory(test)

add_library(${PROJECT_NAME} STATIC src/noise_generator.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	PUBILC_HEADER include/noise_generator.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME}
	PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME} PUBLIC FastNoise)
