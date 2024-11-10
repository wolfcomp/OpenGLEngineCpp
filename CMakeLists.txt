cmake_minimum_required(VERSION 3.21)

if (POLICY CPM0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(OpenGLEngineCpp)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE IMGUI_SOURCES "deps/includes/imgui/*.cpp")
file(GLOB_RECURSE IMGUI_HEADERS "deps/includes/imgui/*.h")

include_directories(deps/includes)

add_executable(OpenGLEngineCpp "app.cpp" "glad.c" ${SOURCES} ${HEADERS} ${IMGUI_SOURCES} ${IMGUI_HEADERS})

target_link_options(OpenGLEngineCpp PRIVATE "/NODEFAULTLIB:library")
target_link_directories(OpenGLEngineCpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/libs)
target_link_libraries(OpenGLEngineCpp PRIVATE "glfw3.lib")
target_link_libraries(OpenGLEngineCpp PRIVATE "opengl32.lib")
target_link_libraries(OpenGLEngineCpp PRIVATE "lua54.lib")

add_custom_command(TARGET OpenGLEngineCpp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:OpenGLEngineCpp>/shaders)
add_custom_command(TARGET OpenGLEngineCpp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/textures $<TARGET_FILE_DIR:OpenGLEngineCpp>/textures)
add_custom_command(TARGET OpenGLEngineCpp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/pointcloud $<TARGET_FILE_DIR:OpenGLEngineCpp>/pointcloud)
add_custom_command(TARGET OpenGLEngineCpp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/scripts $<TARGET_FILE_DIR:OpenGLEngineCpp>/scripts)