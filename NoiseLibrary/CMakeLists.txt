add_subdirectory(test)

add_library(NoiseLibrary STATIC src/noise_generator.cpp)

target_compile_options(NoiseLibrary PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
)

target_include_directories(NoiseLibrary PRIVATE src)
target_include_directories(NoiseLibrary
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)

target_link_libraries(NoiseLibrary PUBLIC FastNoise)
