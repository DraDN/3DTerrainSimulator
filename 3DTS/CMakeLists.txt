cmake_minimum_required(VERSION 3.20.0)
project(3DTS VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.1.0)
find_package(GLEW REQUIRED)
set(SDL2_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14)
find_package(SDL2 REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GraphicalApplicationLibrary-install")
find_package(GraphicalApplicationLibrary CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
							#    src/terrain_generator.cpp
							)
			
target_compile_options(${PROJECT_NAME} PRIVATE -mconsole)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES} GraphicalApplicationLibrary::GraphicalApplicationLibrary Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} SDL2::Main GraphicalApplicationLibrary::GraphicalApplicationLibrary Threads::Threads)

include_directories(include)
add_subdirectory(dependencies)


# target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/dependencies)
