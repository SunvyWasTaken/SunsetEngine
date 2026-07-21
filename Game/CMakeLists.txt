
add_executable(SunsetCraftGame main.cpp)

target_link_libraries(SunsetCraftGame PRIVATE Sunset::Runtime)
if (PRODUCTION_BUILD)
    target_compile_definitions(SunsetCraftGame PUBLIC LOCAL_PATH="./")
else ()
    target_compile_definitions(SunsetCraftGame PUBLIC LOCAL_PATH="${CMAKE_BINARY_DIR}/")
endif ()
