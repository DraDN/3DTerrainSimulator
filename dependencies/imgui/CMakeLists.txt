set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE IMGUI_SRC_FILES ${IMGUI_DIR}/*.cpp)

set(SDL2_PATH ${PROJECT_SOURCE_DIR}/dependencies/SDL2-2.0.14)
find_package(SDL2 REQUIRED)

add_library(imgui STATIC ${IMGUI_SRC_FILES})
target_compile_options(imgui PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
)
# target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SRC_FILES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE ${SDL2_LIBRARIES})
	# $<BUILD_INTERFACE:${IMGUI_DIR}>
	# $<INSTALL_INTERFACE:include/imgui>)