cmake_minimum_required(VERSION 3.28)

add_library(SunsetEngine_compiler_flags INTERFACE)
target_compile_features(SunsetEngine_compiler_flags INTERFACE cxx_std_20)

project(SunsetEngine)

file(GLOB_RECURSE SOURCES "*.h" "*.cpp" "*.txt")

add_subdirectory(Thirdparty/FastNoiseSIMD)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC SAVE_PATH=\"${CMAKE_SOURCE_DIR}/Save/\")
target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_SHADERS_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/Shaders/\")

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(unofficial-enet REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${Stb_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenGL::GL
        glfw
        glad::glad
        imgui::imgui
        unofficial-enet
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        SunsetEngine_compiler_flags
        glm::glm
        nlohmann_json::nlohmann_json
        spdlog::spdlog_header_only
        fastNoiseSIMD
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
        Engine/SunsetPCH.h
)
