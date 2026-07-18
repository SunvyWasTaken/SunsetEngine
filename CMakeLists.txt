cmake_minimum_required(VERSION 3.28)
project(SunsetEngineWorkspace LANGUAGES C CXX)

# Build the editor independently by default.  A game can opt in by exposing the
# `Sunset::ConfigureEditorApplication` entry point documented in README.md.
option(SUNSET_BUILD_EDITOR "Build Sunset Editor" ON)
set(SUNSET_EDITOR_APPLICATION_TARGET "" CACHE STRING
    "Optional target that configures SunsetEditor")
set(SUNSET_EDITOR_APPLICATION_PATH "" CACHE PATH
    "Optional CMake project directory that defines SUNSET_EDITOR_APPLICATION_TARGET")
set(SUNSET_EDITOR_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Editor" CACHE INTERNAL
    "Directory containing the Sunset Editor integration headers")

add_subdirectory(Thirdparty/FastNoiseSIMD)
add_subdirectory(Thirdparty/SRmGUI)
add_subdirectory(Engine)

if(SUNSET_BUILD_EDITOR)
    # This is intentionally added after the engine, so an external application
    # can link SunsetEngine while remaining selected by the build configuration.
    if(SUNSET_EDITOR_APPLICATION_PATH)
        add_subdirectory("${SUNSET_EDITOR_APPLICATION_PATH}"
                         "${CMAKE_BINARY_DIR}/sunset-editor-application")
    endif()

    add_subdirectory(Editor)
endif()
