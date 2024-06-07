# Projects Settings
set(PROJECT_NAME "opengl-cmake-cs")
cmake_minimum_required(VERSION 3.5)
project(${PROJECT_NAME})

# Option to control building dependencies
option(BUILD_DEPENDENCIES "Build external dependencies" ON)

# The main executable
file(GLOB SRC_FILES "src/*.cpp") # 获取src目录下的所有源文件
# 添加可执行文件并将所有源文件链接到该可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

if(BUILD_DEPENDENCIES)
    add_definitions(-DGLEW_STATIC)
    add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
    add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
    add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
    add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE libglew_static
    PRIVATE glm
    PRIVATE assimp
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/asset.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/asset.hpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include
)