file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(imgui CONFIG REQUIRED)

add_executable(SunsetEditor ${MY_SOURCES})

target_include_directories(
        SunsetEditor
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(SunsetEditor PRIVATE
        Sunset::Runtime
        imgui::imgui
)

add_dependencies(SunsetEditor Sunset::Game)

target_compile_definitions(SunsetEditor
        PUBLIC
            LOCAL_PATH="${CMAKE_BINARY_DIR}/"
        PRIVATE
            SUNSET_EDITOR_LOCAL_RESOURCES=\"${CMAKE_CURRENT_SOURCE_DIR}/Resources/\"
            SUNSET_EDITOR_GAME_MODULE_PATH=\"$<TARGET_FILE:${SUNSET_GAME_TARGET}>\"
            SUNSET_EDITOR_GAME_NAME=\"${SUNSET_GAME_TARGET}\"
)
