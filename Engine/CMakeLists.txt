
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(SunsetEngine STATIC)
add_library(Sunset::Engine ALIAS SunsetEngine)

target_sources(
        SunsetEngine
        PRIVATE
            ${MY_SOURCES}
)

target_include_directories(
        SunsetEngine
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)

if(PRODUCTION_BUILD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC SAVE_PATH=\"./Save/\")
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_SHADERS_PATH=\"./Shaders/\")
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC SAVE_PATH=\"${CMAKE_SOURCE_DIR}/Save/\")
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_SHADERS_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/Shaders/\")
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-enet CONFIG REQUIRED)

find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

target_precompile_headers(
        SunsetEngine
        PUBLIC
            SunsetPCH.h
)

target_link_libraries(
        SunsetEngine
        PRIVATE
            glfw
            glad::glad
            spdlog::spdlog_header_only
            unofficial::enet::enet
        PUBLIC
            SunsetEngine_compiler_flags
            glm::glm
            nlohmann_json::nlohmann_json
            fastNoiseSIMD
            EnTT::EnTT
            SRmGUI
)
