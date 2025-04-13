cmake_minimum_required(VERSION 3.20.0)
project(GraphicalApplicationLibrary VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_PATH "./dependencies/SDL2-2.0.14")
find_package(SDL2 REQUIRED)

set(GLM_ROOT_DIR "./dependencies/glm")
# set(GLEW_ROOT_DIR "./dependencies/glew-2.1.0/")
find_package(GLM REQUIRED)

find_package(OpenGL REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "./dependencies/glew-2.1.0")
find_package(GLEW REQUIRED)

set(IMGUI_INCLUDE_DIR "./dependencies/imgui")

# include(GNUInstallDirs)

# add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME} STATIC src/renderer_opengl/buffer.cpp
								   src/renderer_opengl/texture.cpp
								   src/renderer_opengl/vao.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	PUBLIC_HEADER include/gal.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${IMGUI_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE
	PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES})

# install(TARGETS ${PROJECT_NAME}
	# LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDDIR}
	# PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ${PROJECT_NAME} 
	EXPORT ${PROJECT_NAME}
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION export_include)
