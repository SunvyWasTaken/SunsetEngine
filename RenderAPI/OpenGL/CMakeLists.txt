
find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(SunsetBackEndOpenGL STATIC)
add_library(Sunset::BackendOpenGL ALIAS SunsetBackEndOpenGL)

target_sources(
        SunsetBackEndOpenGL
        PRIVATE
            ${MY_SOURCES}
)

target_include_directories(
        SunsetBackEndOpenGL
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
        SunsetBackEndOpenGL
        PUBLIC
            Sunset::Engine
        PRIVATE
            glad::glad
            OpenGL::GL
)
