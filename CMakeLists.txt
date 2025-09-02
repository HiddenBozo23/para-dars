cmake_minimum_required(VERSION 3.21)
project(para-dars VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad::glad
    glm::glm-header-only
    assimp::assimp
    imgui::imgui
    imguizmo::imguizmo
)

#copy shaders into build
set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/src/graphics/shaders")
add_custom_command(TARGET para-dars POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SHADERS_DIR}"
        "$<TARGET_FILE_DIR:para-dars>/shaders"
)

#copy assets into build
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
add_custom_command(TARGET para-dars POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSETS_DIR}"
    "$<TARGET_FILE_DIR:para-dars>/assets"
)