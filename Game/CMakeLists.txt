
add_library(SunsetGameTarget INTERFACE)
add_library(Sunset::Game ALIAS SunsetGameTarget)

# Part to change to start ur project
# ex:
# add_subdirectory(yourproject)
# add_dependencies(SunsetGameTarget yourproject)

add_subdirectory(Pong)
add_dependencies(SunsetGameTarget Pong)

# ----------------------------------------------

add_executable(SunsetGame main.cpp)

target_link_libraries(SunsetGame PRIVATE Sunset::Runtime)

if (PRODUCTION_BUILD)
    target_compile_definitions(SunsetGame PUBLIC LOCAL_PATH="./")
else ()
    target_compile_definitions(SunsetGame PUBLIC LOCAL_PATH="${CMAKE_BINARY_DIR}/")
endif ()

add_dependencies(SunsetGame Sunset::Game)

target_compile_definitions(SunsetGame PRIVATE
        SUNSET_GAME_MODULE_PATH=\"$<TARGET_FILE:${SUNSET_GAME_TARGET}>\"
        SUNSET_GAME_NAME="${SUNSET_GAME_TARGET}"
)
