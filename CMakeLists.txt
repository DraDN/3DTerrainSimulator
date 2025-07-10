cmake_minimum_required(VERSION 3.20)
project(3DTS VERSION 0.1.0)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_BUILD_TYPE Debug)

# set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} -g -O0)
# set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3DTS)
# set(SOURCE_DIR2 ${CMAKE_CURRENT_SOURCE_DIR}/GraphicalApplicationLibrary)
# set(SOURCE_DIR3 ${CMAKE_CURRENT_SOURCE_DIR}/NoiseLibrary)
# set(SOURCE_DIR4 ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui)
# file(GLOB_RECURSE SRC_FILES ${SOURCE_DIR}/*.cpp)
# file(GLOB_RECURSE SRC_FILES2 ${SOURCE_DIR2}/*.cpp)
# file(GLOB_RECURSE SRC_FILES3 ${SOURCE_DIR3}/*.cpp)
# file(GLOB_RECURSE SRC_FILES4 ${SOURCE_DIR4}/*.cpp)

# find_package(Threads REQUIRED)
# find_package(OpenGL REQUIRED)
# list(APPEND CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/dependencies/glew-2.1.0)
# find_package(GLEW REQUIRED)
# set(SDL2_PATH ${PROJECT_SOURCE_DIR}/dependencies/SDL2-2.0.14)
# find_package(SDL2 REQUIRED)

# add_executable(3DTS ${SRC_FILES} ${SRC_FILES2} ${SRC_FILES3} ${SRC_FILES4})
			
# target_compile_options(3DTS PUBLIC -mconsole)
# target_compile_features(3DTS PUBLIC cxx_std_20)

# target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES} GraphicalApplicationLibrary::GraphicalApplicationLibrary Threads::Threads)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} SDL2::Main GraphicalApplicationLibrary::GraphicalApplicationLibrary Threads::Threads)
# target_link_libraries(3DTS PRIVATE GraphicalApplicationLibrary NoiseLibrary Threads::Threads)
# target_link_libraries(3DTS PRIVATE Threads::Threads)
add_subdirectory(dependencies/FastNoise2)

# target_include_directories(3DTS PRIVATE 3DTS/include NoiseLibrary/include GraphicalApplicationLibrary/include dependencies/glm/ dependencies/imgui ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
# target_link_libraries(3DTS PRIVATE ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES} FastNoise)

# add_custom_target(copy_resources ALL
# 	COMMAND ${CMAKE_COMMAND} -E copy_directory
# 		${CMAKE_CURRENT_SOURCE_DIR}/res
# 		${CMAKE_BINARY_DIR}/res
# )
enable_testing()

add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/imgui)

add_subdirectory(GraphicalApplicationLibrary)
add_subdirectory(NoiseLibrary)

add_subdirectory(3DTS)
