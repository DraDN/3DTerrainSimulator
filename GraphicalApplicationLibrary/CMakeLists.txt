add_library(GraphicalApplicationLibrary STATIC 
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

target_include_directories(GraphicalApplicationLibrary
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(GraphicalApplicationLibrary PUBLIC cxx_std_20)
target_compile_options(GraphicalApplicationLibrary PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
)

find_package(OpenGL REQUIRED)
list(APPEND CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/dependencies/glew-2.1.0)
find_package(GLEW REQUIRED)
set(SDL2_PATH ${PROJECT_SOURCE_DIR}/dependencies/SDL2-2.0.14)
find_package(SDL2 REQUIRED)

target_link_libraries(GraphicalApplicationLibrary
    PUBLIC
        OpenGL::GL
        GLEW::GLEW
        SDL2::Main
		glm
        imgui
)