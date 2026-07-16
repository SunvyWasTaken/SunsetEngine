cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

project(SunsetEditor)

find_package(imgui CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${MY_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC SunsetEngine)

target_link_libraries(${PROJECT_NAME} PRIVATE
        SunsetCraft
        imgui::imgui
    )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
