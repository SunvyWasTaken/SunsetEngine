cmake_minimum_required(VERSION 3.28)

add_library(SunsetEngine_compiler_flags INTERFACE)
target_compile_features(SunsetEngine_compiler_flags INTERFACE cxx_std_20)

target_compile_definitions(SunsetEngine_compiler_flags INTERFACE
        $<$<CONFIG:Debug>:SS_PROFILING>
        $<$<CONFIG:RelWithDebInfo>:SS_PROFILING>
        $<$<CONFIG:Release>:SS_RELEASE>
)

project(SunsetEngine)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Engine/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Engine/*.cpp")

add_subdirectory(Thirdparty/FastNoiseSIMD)
add_subdirectory(Thirdparty/SRmGUI)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

if(PRODUCTION_BUILD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC SAVE_PATH=\"./Save/\")
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_SHADERS_PATH=\"./Shaders/\")
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC SAVE_PATH=\"${CMAKE_SOURCE_DIR}/Save/\")
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_SHADERS_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/Shaders/\")
endif()

find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(unofficial-enet CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        glad::glad
        spdlog::spdlog_header_only
        imgui::imgui
        unofficial::enet::enet
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        SunsetEngine_compiler_flags
        glm::glm
        nlohmann_json::nlohmann_json
        fastNoiseSIMD
        EnTT::EnTT
        SRmGUI
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
        Engine/SunsetPCH.h
)
