cmake_minimum_required(VERSION 3.28)

project(
    SunsetEngine
    VERSION 0.2.0
    LANGUAGES CXX
)

#! ! ! ! ! ! !
#set this to ON instead of OFF to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
#delete the out folder after changing if visual studio doesn't recognize the change
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

add_library(SunsetEngine_compiler_flags INTERFACE)

target_compile_features(SunsetEngine_compiler_flags INTERFACE cxx_std_23)

target_compile_definitions(SunsetEngine_compiler_flags INTERFACE
        $<$<CONFIG:Debug>:SS_PROFILING>
        $<$<CONFIG:Debug>:SS_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:SS_PROFILING>
        $<$<CONFIG:RelWithDebInfo>:SS_DEBUG>
        $<$<CONFIG:Release>:SS_RELEASE>
)

if(PRODUCTION_BUILD)
    target_compile_definitions(SunsetEngine_compiler_flags INTERFACE SAVE_PATH=\"./Save\")
else ()
    target_compile_definitions(SunsetEngine_compiler_flags INTERFACE SAVE_PATH=\"${CMAKE_SOURCE_DIR}/Save/\")
endif ()

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
    target_compile_definitions(SunsetEngine_compiler_flags INTERFACE SUNSET_OPENGL)
    target_link_libraries(SunsetRuntime INTERFACE Sunset::BackendOpenGL)
endif ()

if (SUNSET_USE_VULKAN)
    target_compile_definitions(SunsetEngine_compiler_flags INTERFACE SUNSET_VULKAN)
    target_link_libraries(SunsetRuntime INTERFACE Sunset::Vulkan)
endif ()

add_subdirectory(Game)

if (SUNSET_BUILD_EDITOR)
    add_subdirectory(Editor)
endif ()
