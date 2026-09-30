file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(unofficial-imgui-node-editor CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

add_executable(SunsetEditor ${MY_SOURCES})

target_include_directories(
        SunsetEditor
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(SunsetEditor PRIVATE
        Sunset::Runtime
        imgui::imgui
        imguizmo::imguizmo
        unofficial::imgui-node-editor::imgui-node-editor
        yaml-cpp::yaml-cpp
)

add_dependencies(SunsetEditor Sunset::Game)

if(PRODUCTION_BUILD)
    target_compile_definitions(SunsetEditor
            PUBLIC
                LOCAL_PATH="${CMAKE_BINARY_DIR}/"
            PRIVATE
                EDITOR_RESOURCES=\"Resources/\"
                SUNSET_RESOURCES=\"Resources/\"
                CONTENT_PATH=\"./Content/
                SUNSET_EDITOR_GAME_MODULE_PATH=\"$<TARGET_FILE:${SUNSET_GAME_TARGET}>\"
                SUNSET_EDITOR_GAME_NAME=\"${SUNSET_GAME_TARGET}\"
    )
else()
    target_compile_definitions(SunsetEditor
            PUBLIC
                LOCAL_PATH="${CMAKE_BINARY_DIR}/"
            PRIVATE
                EDITOR_RESOURCES=\"${CMAKE_CURRENT_SOURCE_DIR}/Resources/\"
                SUNSET_RESOURCES=\"${CMAKE_SOURCE_DIR}/Resources/\"
                CONTENT_PATH=\"${CMAKE_SOURCE_DIR}/Content/\"
                SUNSET_EDITOR_GAME_MODULE_PATH=\"$<TARGET_FILE:${SUNSET_GAME_TARGET}>\"
                SUNSET_EDITOR_GAME_NAME=\"${SUNSET_GAME_TARGET}\"
    )
endif()
