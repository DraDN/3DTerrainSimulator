target_include_directories(${PROJECT_NAME} PRIVATE include)
add_subdirectory(dependencies/FastNoise2)
target_link_libraries(${PROJECT_NAME} PUBLIC FastNoise)
target_link_libraries(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/libNoiseLibrary.a")