cmake_minimum_required (VERSION 3.16)

project (Object-Viewer)

set (source_dir "${PROJECT_SOURCE_DIR}/src/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# Unfortunately, since Windows lacks a default directory structure like /usr/local/lib,
# it is hard to come up with reasonable defaults for searching here.

find_package(OpenGL REQUIRED)
add_subdirectory(${source_dir}/deps/glfw)

include_directories(${source_dir})
include_directories(${source_dir}/deps/glm)
include_directories(${source_dir}/deps/glfw/include)
include_directories(${source_dir}/deps/glew/include)
include_directories(${source_dir}/deps/freeglut/include)
include_directories(${source_dir}/deps/include)
include_directories(${source_dir}/deps/imgui)
include_directories(${source_dir}/deps/openmesh/include)

link_directories(${source_dir}/deps/glew/lib/Release/x64)
link_directories(${source_dir}/deps/libs)
link_directories(${source_dir}/deps/freeglut/lib)
link_directories(${source_dir}/deps/glfw/lib)
link_directories(${source_dir}/deps/openmesh/lib)

file (GLOB source_files "${source_dir}/*.cpp" "${source_dir}/deps/*.cpp" "${source_dir}/deps/*/*.cpp" "${source_dir}/deps/imgui/backends/*.cpp")

add_executable (viewer ${source_files})

message(WARNING "${GLFW_LIBRARIES}")

target_link_libraries(viewer OpenGL::GL)
target_link_libraries(viewer glew32 glfw3)
target_link_libraries(viewer OpenMeshCore OpenMeshTools)
