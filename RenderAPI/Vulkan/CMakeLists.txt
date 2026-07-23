
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(Vulkan CONFIG REQUIRED)

add_library(SunsetBackEndVulkan STATIC)
add_library(Sunset::Vulkan ALIAS SunsetBackEndVulkan)

target_sources(
        SunsetBackEndVulkan
        PRIVATE
            ${MY_SOURCES}
)

target_include_directories(
        SunsetBackEndVulkan
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
        SunsetBackEndVulkan
        PUBLIC
        Sunset::Engine
        PRIVATE
            vulkan
)
