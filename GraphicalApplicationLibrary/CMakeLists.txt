# cmake_minimum_required(VERSION 3.20.0)
# project(GraphicalApplicationLibrary VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 20)

# # include(GNUInstallDirs)

# # add_library(${PROJECT_NAME} INTERFACE)
# add_library(${PROJECT_NAME} STATIC src/renderer_opengl/buffer.cpp
# 								   src/renderer_opengl/texture.cpp
# 								   src/renderer_opengl/vao.cpp
# 								   src/renderer_opengl/camera.cpp
# 								   src/renderer_opengl/light.cpp
# 								   src/renderer_opengl/render_target.cpp
# 								   src/renderer_opengl/renderer.cpp
# 								   src/renderer_opengl/shader.cpp
# 								   src/renderer_opengl/texture.cpp
# 								   src/renderer_opengl/model.cpp
# 								   src/ui_imgui/implementation/sdl/ui_context_sdl.cpp
# 								   src/window_sdl.cpp
# 								   src/graphics_app_sdl_opengl.cpp)

# # set_target_properties(${PROJECT_NAME} PROPERTIES 
# 	# VERSION ${PROJECT_VERSION}
# 	# SOVERSION ${PROJECT_VERSION_MAJOR}
# 	# PUBLIC_HEADER include/gal.hpp)

# # --- GLEW ---
# add_library(GLEW::GLEW STATIC IMPORTED)
# set_target_properties(GLEW::GLEW PROPERTIES
# 	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.1.0/lib/Release/x64/glew32s.lib"
# 	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.1.0/include")
# # list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.1.0)
# # find_package(GLEW REQUIRED)

# # --- SDL2 ---
# add_library(SDL2 STATIC IMPORTED)
# set_target_properties(SDL2 PROPERTIES
# 	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14/lib/libSDL2.a"
# 	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14/include")
# # set(SDL2_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14)
# # find_package(SDL2 REQUIRED)

# # --- GLM ---
# add_library(GLM INTERFACE IMPORTED)
# set_target_properties(GLM PROPERTIES
# 	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm")
# # set(GLM_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm)
# # set(GLEW_ROOT_DIR "./dependencies/glew-2.1.0/")
# # find_package(GLM REQUIRED)

# # --- ImGui ---
# add_library(ImGui STATIC
# 	dependencies/imgui/imgui.cpp
# 	dependencies/imgui/imgui_draw.cpp
# 	dependencies/imgui/imgui_tables.cpp
# 	dependencies/imgui/imgui_widgets.cpp
# 	dependencies/imgui/imgui_impl_opengl3.cpp
# 	dependencies/imgui/imgui_impl_sdl.cpp
# 	dependencies/imgui/imgui_demo.cpp
# )
# target_include_directories(ImGui PUBLIC
# 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui>
# 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14/include/SDL2>
# 	$<INSTALL_INTERFACE:include/imgui>)

# find_package(OpenGL REQUIRED)

# # set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui)
# target_link_libraries(${PROJECT_NAME}
# 	PUBLIC
# 		GLEW::GLEW
# 		SDL2
# 		GLM
# 		OpenGL::GL
# 		ImGui
# )

# # target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
# # target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${IMGUI_INCLUDE_DIR})
# # target_include_directories(${PROJECT_NAME} PRIVATE
# 	# ${GLEW_INCLUDE_DIRS}
# 	# ${GLM_INCLUDE_DIRS}
# 	# ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui
# 	# ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14/include)
# # target_include_directories(${PROJECT_NAME}
# #     PUBLIC
# #         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
# #         $<INSTALL_INTERFACE:include>
# #         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm>
# #         $<INSTALL_INTERFACE:include/glm>
# #         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui>
# #         $<INSTALL_INTERFACE:include/imgui>
# #         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14/include>
# #         $<INSTALL_INTERFACE:include/SDL2>
# #         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.1.0/include>
# #         $<INSTALL_INTERFACE:include/glew>
# # )

# target_include_directories(${PROJECT_NAME}
# 	PUBLIC
# 		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
# 		$<INSTALL_INTERFACE:include>)

# # target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES})

# # install(TARGETS ${PROJECT_NAME}
# 	# LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDDIR}
# 	# PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install(TARGETS ${PROJECT_NAME} 
# 	EXPORT GraphicalApplicationLibraryTargets
# 	ARCHIVE DESTINATION lib
# 	LIBRARY DESTINATION lib
# 	RUNTIME DESTINATION bin)

# install(TARGETS ImGui
# 	EXPORT GraphicalApplicationLibraryTargets)

# install(DIRECTORY include/ DESTINATION include)
# install(DIRECTORY dependencies/imgui DESTINATION include/imgui
# 	FILES_MATCHING PATTERN "*.h")
# install(DIRECTORY dependencies/glm DESTINATION include/glm)
# install(DIRECTORY dependencies/imgui DESTINATION include/imgui)
# install(DIRECTORY dependencies/SDL2-2.0.14/include DESTINATION include/SDL2)
# install(DIRECTORY dependencies/glew-2.1.0/include DESTINATION include/glew)

# install(EXPORT GraphicalApplicationLibraryTargets
# 	FILE GraphicalApplicationLibraryTargets.cmake
# 	NAMESPACE GraphicalApplicationLibrary::
# 	DESTINATION lib/cmake/GraphicalApplicationLibrary)

# include(CMakePackageConfigHelpers)

# write_basic_package_version_file(
# 	"${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfigVersion.cmake"
# 	VERSION 1.0.0
# 	COMPATIBILITY AnyNewerVersion
# )

# configure_package_config_file(
# 	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/GraphicalApplicationLibraryConfig.cmake.in"
# 	"${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfig.cmake"
# 	INSTALL_DESTINATION lib/cmake/GraphicalApplicationLibrary
# )

# install(
#     FILES
#         "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfig.cmake"
#         "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfigVersion.cmake"
#     DESTINATION lib/cmake/GraphicalApplicationLibrary
# )

# ===========
# cmake_minimum_required(VERSION 3.20.0)
# project(GraphicalApplicationLibrary VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 20)

# add_library(${PROJECT_NAME} STATIC 
#     src/renderer_opengl/buffer.cpp
#     src/renderer_opengl/texture.cpp
#     src/renderer_opengl/vao.cpp
#     src/renderer_opengl/camera.cpp
#     src/renderer_opengl/light.cpp
#     src/renderer_opengl/render_target.cpp
#     src/renderer_opengl/renderer.cpp
#     src/renderer_opengl/shader.cpp
#     src/renderer_opengl/texture.cpp
#     src/renderer_opengl/model.cpp
#     src/ui_imgui/implementation/sdl/ui_context_sdl.cpp
#     src/window_sdl.cpp
#     src/graphics_app_sdl_opengl.cpp
# )

# # Include directories for your project's headers
# target_include_directories(${PROJECT_NAME}
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#         $<INSTALL_INTERFACE:include>
# )

# # Handle dependencies' includes and libraries

# # OpenGL
# find_package(OpenGL REQUIRED)
# target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)

# # --- GLEW ---
# target_include_directories(${PROJECT_NAME} PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.1.0/include>
#     $<INSTALL_INTERFACE:include/glew>
# )
# target_link_libraries(${PROJECT_NAME} PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.1.0/lib/Release/x64/glew32.lib
# )
# add_definitions(-DGLEW_STATIC)

# # --- SDL2 ---
# target_include_directories(${PROJECT_NAME} PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14/include>
#     $<INSTALL_INTERFACE:include/SDL2>
# )
# target_link_libraries(${PROJECT_NAME} PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14/lib/libSDL2.dll.a
# )

# # --- GLM ---
# target_include_directories(${PROJECT_NAME} PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm>
#     $<INSTALL_INTERFACE:include/glm>
# )

# # --- ImGui ---
# add_library(ImGui STATIC
#     dependencies/imgui/imgui.cpp
#     dependencies/imgui/imgui_draw.cpp
#     dependencies/imgui/imgui_tables.cpp
#     dependencies/imgui/imgui_widgets.cpp
#     dependencies/imgui/imgui_impl_opengl3.cpp
#     dependencies/imgui/imgui_impl_sdl.cpp
# )
# target_include_directories(ImGui PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui>
# 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14/include/SDL2>
#     $<INSTALL_INTERFACE:include/imgui>
# )

# # Link ImGui
# target_link_libraries(${PROJECT_NAME} PUBLIC ImGui)

# # Install targets
# install(TARGETS ${PROJECT_NAME} ImGui
#     EXPORT GraphicalApplicationLibraryTargets
#     ARCHIVE DESTINATION lib
#     LIBRARY DESTINATION lib
#     RUNTIME DESTINATION bin
#     PUBLIC_HEADER DESTINATION include
# )

# # Install headers
# install(DIRECTORY include/ DESTINATION include
# 	FILES_MATCHING PATTERN "*.hpp")
# install(DIRECTORY dependencies/imgui DESTINATION include
#     FILES_MATCHING PATTERN "*.h")
# install(DIRECTORY dependencies/glm/glm DESTINATION include/glm)
# install(DIRECTORY dependencies/SDL2-2.0.14/include/ DESTINATION include/SDL2)
# install(DIRECTORY dependencies/glew-2.1.0/include/ DESTINATION include/glew)

# # Install static libraries of dependencies
# install(FILES 
#     dependencies/glew-2.1.0/lib/Release/x64/glew32s.lib
#     dependencies/SDL2-2.0.14/lib/libSDL2.a
#     DESTINATION lib
# )

# # Export targets
# install(EXPORT GraphicalApplicationLibraryTargets
#     FILE GraphicalApplicationLibraryTargets.cmake
#     NAMESPACE GraphicalApplicationLibrary::
#     DESTINATION lib/cmake/GraphicalApplicationLibrary
# )

# # Package configuration
# include(CMakePackageConfigHelpers)
# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfigVersion.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )

# configure_package_config_file(
#     cmake/GraphicalApplicationLibraryConfig.cmake.in
#     "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfig.cmake"
#     INSTALL_DESTINATION lib/cmake/GraphicalApplicationLibrary
# )

# install(
#     FILES
#         "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfig.cmake"
#         "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfigVersion.cmake"
#     DESTINATION lib/cmake/GraphicalApplicationLibrary
# )

# =======
cmake_minimum_required(VERSION 3.14)
project(GraphicalApplicationLibrary VERSION 1.0.0)

# Create the library
add_library(${PROJECT_NAME} STATIC 
    # src/renderer_opengl/buffer.cpp
    src/renderer_opengl/texture.cpp
    src/renderer_opengl/vao.cpp
    src/renderer_opengl/camera.cpp
    src/renderer_opengl/light.cpp
    src/renderer_opengl/render_target.cpp
    src/renderer_opengl/renderer.cpp
    src/renderer_opengl/shader.cpp
    src/renderer_opengl/texture.cpp
    src/renderer_opengl/model.cpp
    src/ui_imgui/implementation/sdl/ui_context_sdl.cpp
    src/window_sdl.cpp
    src/graphics_app_sdl_opengl.cpp
)

# # Public include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(GraphicalApplicationLibrary PUBLIC cxx_std_20)
# set(CMAKE_CXX_STANDARD 20)

# # Find external dependencies
find_package(OpenGL REQUIRED)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.1.0)
find_package(GLEW REQUIRED)
set(SDL2_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2-2.0.14)
find_package(SDL2 REQUIRED)
# # find_package(GLM REQUIRED)
add_subdirectory(dependencies/glm)

# # If you vendored ImGui inside your project manually:
# # You would also set up imgui here manually by source files and include.
add_subdirectory(dependencies/imgui)

# # Public linking of dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        OpenGL::GL
        GLEW::GLEW
        SDL2::Core
		glm
)

# # target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_INCLUDE_DIRS})

install(TARGETS glm
EXPORT GraphicalApplicationLibraryTargets)

install(TARGETS ${PROJECT_NAME}
    EXPORT GraphicalApplicationLibraryTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# # Install rules
install(DIRECTORY include/ DESTINATION include)
include(CMakePackageConfigHelpers)
install(DIRECTORY dependencies/imgui DESTINATION include
FILES_MATCHING PATTERN "*.h")
install(DIRECTORY dependencies/glm/glm DESTINATION include)

install(EXPORT GraphicalApplicationLibraryTargets
    FILE GraphicalApplicationLibraryTargets.cmake
    NAMESPACE GraphicalApplicationLibrary::
    DESTINATION lib/cmake/GraphicalApplicationLibrary
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/GraphicalApplicationLibraryConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfig.cmake"
    INSTALL_DESTINATION lib/cmake/GraphicalApplicationLibrary
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/GraphicalApplicationLibraryConfigVersion.cmake"
    DESTINATION lib/cmake/GraphicalApplicationLibrary
)
