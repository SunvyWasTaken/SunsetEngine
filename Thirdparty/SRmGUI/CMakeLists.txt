cmake_minimum_required(VERSION 3.28)

project(SRmGUI)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC ${MY_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
    glm::glm
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
        SRmGUI_PCH.h
)
