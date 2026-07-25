cmake_minimum_required(VERSION 3.28)

project(
    SunsetEngine
    VERSION 0.1.0
    LANGUAGES CXX
)

add_library(SunsetEngine_compiler_flags INTERFACE)
target_compile_features(SunsetEngine_compiler_flags INTERFACE cxx_std_23)
target_compile_definitions(SunsetEngine_compiler_flags INTERFACE
        $<$<CONFIG:Debug>:SS_PROFILING>
        $<$<CONFIG:RelWithDebInfo>:SS_PROFILING>
        $<$<CONFIG:Release>:SS_RELEASE>
)

# Permet de faire des options.
option(SUNSET_BUILD_EDITOR "Build Sunset Editor" ON)
option(SUNSET_USE_GLFW "Use GLFW as Window platform" ON)
option(SUNSET_USE_OPENGL "Use OpenGL as Rendering backend" ON)
option(SUNSET_USE_VULKAN "Use Vulkan as Rendering backend" OFF)

add_subdirectory(Thirdparty/FastNoiseSIMD)
add_subdirectory(Thirdparty/SRmGUI)

add_subdirectory(Engine)

add_subdirectory(Platform)
add_subdirectory(RenderAPI)

add_library(SunsetRuntime INTERFACE)
add_library(Sunset::Runtime ALIAS SunsetRuntime)

target_link_libraries(SunsetRuntime INTERFACE Sunset::Engine)

if (SUNSET_USE_GLFW)
    target_link_libraries(SunsetRuntime INTERFACE Sunset::PlatformGLFW)
endif ()

if (SUNSET_USE_OPENGL)
    target_link_libraries(SunsetRuntime INTERFACE Sunset::BackendOpenGL)
endif ()

if (SUNSET_USE_VULKAN)
    target_link_libraries(SunsetRuntime INTERFACE Sunset::Vulkan)
endif ()

if (SUNSET_BUILD_EDITOR)
    add_subdirectory(Editor)
endif ()

add_subdirectory(Game)
