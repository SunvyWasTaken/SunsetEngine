cmake_minimum_required(VERSION 3.28)

add_library(SunsetEngine_compiler_flags INTERFACE)
target_compile_features(SunsetEngine_compiler_flags INTERFACE cxx_std_23)

project(
    SunsetEngine
    VERSION 0.1.0
    LANGUAGES CXX
)

# Permet de faire des options.
option(SUNSET_BUILD_EDITOR "Build Sunset Editor" ON)
option(SUNSET_USE_GLFW "Use GLFW as Window platform" ON)
option(SUNSET_USE_OPENGL "Use OpenGL as Rendering backend" ON)

add_subdirectory(Thirdparty/FastNoiseSIMD)
add_subdirectory(Thirdparty/SRmGUI)

add_subdirectory(Engine)

add_subdirectory(Platform)
add_subdirectory(RenderAPI)

add_library(SunsetRuntime INTERFACE)

target_link_libraries(
        SunsetRuntime
        INTERFACE
            Sunset::Engine
            Sunset::PlatformGLFW
            Sunset::BackendOpenGL
)

add_subdirectory(Editor)

