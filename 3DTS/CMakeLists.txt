find_package(Threads REQUIRED)

add_executable(3DTS src/main.cpp
					src/terrain_generator.cpp
					src/log.cpp
					src/ui.cpp
)
			
target_compile_features(3DTS PUBLIC cxx_std_20)

target_link_libraries(3DTS PRIVATE GraphicalApplicationLibrary NoiseLibrary Threads::Threads spdlog::spdlog)

target_include_directories(3DTS PRIVATE include)

add_custom_target(copy_resources ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/res
		${CMAKE_BINARY_DIR}/3DTS/res
)

add_custom_target(copy_dlls ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/bin
		${CMAKE_BINARY_DIR}/3DTS
)