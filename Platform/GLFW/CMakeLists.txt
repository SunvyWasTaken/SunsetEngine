
find_package(glfw3 CONFIG REQUIRED)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(SunsetPlatformGLFW STATIC)
add_library(Sunset::PlatformGLFW ALIAS SunsetPlatformGLFW)

target_sources(
        SunsetPlatformGLFW
        PRIVATE
            ${MY_SOURCES}
)

target_include_directories(
        SunsetPlatformGLFW
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
        SunsetPlatformGLFW
        PUBLIC
            Sunset::Engine
        PRIVATE
            glfw
)
