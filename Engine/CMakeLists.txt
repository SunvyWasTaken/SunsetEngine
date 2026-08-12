
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

set_target_properties(SunsetEngine PROPERTIES
        PREFIX ""
        OUTPUT_NAME "SunsetEngine"
)


if(PRODUCTION_BUILD)
    target_compile_definitions(SunsetEngine PRIVATE ENGINE_SHADERS_PATH=\"./Shaders/\")
else()
    target_compile_definitions(SunsetEngine PRIVATE ENGINE_SHADERS_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/Shaders/\")
endif()

target_precompile_headers(
        SunsetEngine
        PUBLIC
            SunsetPCH.h
)

find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

find_package(OpenAL CONFIG REQUIRED)
find_package(unofficial-enet CONFIG REQUIRED)
find_package(SndFile REQUIRED)

target_link_libraries(
        SunsetEngine
        PUBLIC
            SunsetEngine_compiler_flags
            glm::glm
            fastNoiseSIMD
            EnTT::EnTT
            SRmGUI
            spdlog::spdlog_header_only
            fmt::fmt
        PRIVATE
            OpenAL::OpenAL
            unofficial::enet::enet
            SndFile::sndfile
)
