cmake_minimum_required(VERSION 4.0)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

project(SunsetEditor)

add_executable(${PROJECT_NAME} ${MY_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC SunsetEngine)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
