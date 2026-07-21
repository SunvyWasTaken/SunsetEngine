cmake_minimum_required(VERSION 3.28)

project(SRmGUI)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC ${MY_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

if(PRODUCTION_BUILD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC SRmGUI_RESOURCES=\"./Resources/\")
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC SRmGUI_RESOURCES=\"${CMAKE_CURRENT_SOURCE_DIR}/Resources/\")
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE
    glm::glm
    glad::glad
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Impl/OpenGL/Include
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
        SRmGUI_PCH.h
)
