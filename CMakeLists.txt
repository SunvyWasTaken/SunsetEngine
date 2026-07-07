cmake_minimum_required(VERSION 3.28)

add_subdirectory(Thirdparty/FastNoiseSIMD)
add_subdirectory(Thirdparty/SRmGUI)
add_subdirectory(Engine)
add_subdirectory(Editor)

# Permet de faire des options.
option(SUNSET_BUILD_EDITOR "Build Sunset Editor" ON)
