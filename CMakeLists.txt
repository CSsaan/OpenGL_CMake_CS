# Projects Settings
set(PEOJECT_NAME "opengl-cmake-cs")
cmake_minimum_required (VERSION 3.1)
project (${PEOJECT_NAME})

# The main executable
file(GLOB SRC_FILES "src/*.cpp") # 获取src目录下的所有源文件
# 添加可执行文件并将所有源文件链接到该可执行文件
add_executable(${PEOJECT_NAME} ${SRC_FILES})


set_property(TARGET ${PEOJECT_NAME} PROPERTY CXX_STANDARD 11)
target_compile_options(${PEOJECT_NAME} PRIVATE -Wall)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)

target_link_libraries(${PEOJECT_NAME}
  PRIVATE glfw
  PRIVATE libglew_static
  PRIVATE glm
  PRIVATE assimp
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/src/asset.hpp
)
target_include_directories(${PEOJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)
