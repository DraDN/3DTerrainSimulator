add_library(glm INTERFACE)
# target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
# add_subdirectory(glm)
target_include_directories(glm INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/glm>
)
