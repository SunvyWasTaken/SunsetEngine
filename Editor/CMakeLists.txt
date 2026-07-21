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

target_compile_definitions(SunsetEditor PUBLIC LOCAL_PATH="$<TARGET_FILE_DIR:SunsetEditor>/")
