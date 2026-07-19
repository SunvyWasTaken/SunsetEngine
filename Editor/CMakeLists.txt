file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

project(SunsetEditor)

find_package(imgui CONFIG REQUIRED)

add_executable(SunsetEditor ${MY_SOURCES})

target_link_libraries(SunsetEditor PRIVATE
        SunsetRuntime
        imgui::imgui
)

target_include_directories(SunsetEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
