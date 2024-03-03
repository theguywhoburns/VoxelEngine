cmake_minimum_required(VERSION 3.22)

project(game
    VERSION 0.0.1
    LANGUAGES C CXX
)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*"
)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/GLFW")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glew/build/cmake")

add_executable(game ${SRCS})
set_property(TARGET game PROPERTY C_STANDARD 17)
set_property(TARGET game PROPERTY CXX_STANDARD 17)
set_property(TARGET game PROPERTY C_STANDARD_REQUIRED true)
set_property(TARGET game PROPERTY CXX_STANDARD_REQUIRED true)

target_link_libraries(game PRIVATE OpenGL::GL)
target_link_libraries(game PRIVATE glfw)
target_link_libraries(game PRIVATE glew_s)
target_include_directories(game PUBLIC 
    "${PROJECT_SOURCE_DIR}/lib/GLFW/include"
    "${PROJECT_SOURCE_DIR}/lib/glew/include"
    "${PROJECT_SOURCE_DIR}/src"
)