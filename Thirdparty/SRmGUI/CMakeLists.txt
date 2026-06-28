cmake_minimum_required(VERSION 3.28)

project(SRmGUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC ${MY_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    glm::glm
    glad::glad
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
        SRmGUI_PCH.h
)
