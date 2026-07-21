
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(SunsetEngine SHARED)
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
    target_compile_definitions(SunsetEngine PUBLIC SAVE_PATH=\"./Save/\")
    target_compile_definitions(SunsetEngine PRIVATE ENGINE_SHADERS_PATH=\"./Shaders/\")
else()
    target_compile_definitions(SunsetEngine PUBLIC SAVE_PATH=\"${CMAKE_SOURCE_DIR}/Save/\")
    target_compile_definitions(SunsetEngine PRIVATE ENGINE_SHADERS_PATH=\"${CMAKE_SOURCE_DIR}/Shaders/\")
endif()

target_precompile_headers(
        SunsetEngine
        PUBLIC
            SunsetPCH.h
)

find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-enet CONFIG REQUIRED)

target_link_libraries(
        SunsetEngine
        PUBLIC
            SunsetEngine_compiler_flags
            glm::glm
            nlohmann_json::nlohmann_json
            fastNoiseSIMD
            EnTT::EnTT
            SRmGUI
            spdlog::spdlog_header_only
            fmt::fmt
        PRIVATE
            unofficial::enet::enet
)
