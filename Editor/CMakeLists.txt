cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

project(SunsetEditor)

find_package(imgui CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${MY_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC SunsetEngine)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

if(SUNSET_EDITOR_APPLICATION_TARGET)
    if(NOT TARGET "${SUNSET_EDITOR_APPLICATION_TARGET}")
        message(FATAL_ERROR
            "SUNSET_EDITOR_APPLICATION_TARGET ('${SUNSET_EDITOR_APPLICATION_TARGET}') does not name an existing CMake target. "
            "Define it before adding SunsetEngine, or set SUNSET_EDITOR_APPLICATION_PATH to a directory that defines it.")
    endif()

    target_link_libraries(${PROJECT_NAME} PRIVATE "${SUNSET_EDITOR_APPLICATION_TARGET}")
    target_compile_definitions(${PROJECT_NAME} PRIVATE SUNSET_EDITOR_HAS_APPLICATION=1)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
