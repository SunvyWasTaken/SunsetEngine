project(SunsetEngine)

file(GLOB_RECURSE SOURCES "*.h" "*.cpp" "*.txt")

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${Stb_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenGL::GL
        glfw
        glad::glad
        imgui::imgui
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        MiniMotor_compiler_flags
        glm::glm
        nlohmann_json::nlohmann_json
        spdlog::spdlog_header_only
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
        SunsetPCH.h
)
