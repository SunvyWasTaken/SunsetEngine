
add_library(SunsetGameTarget INTERFACE)
add_library(Sunset::Game ALIAS SunsetGameTarget)

# Part to change to start ur project
# ex:
# add_subdirectory(yourproject)
# add_dependencies(SunsetGameTarget yourproject)

add_subdirectory(FlapFlap)
add_dependencies(SunsetGameTarget FlapFlap)

# ----------------------------------------------

add_executable(SunsetGame main.cpp)

target_link_libraries(SunsetGame PRIVATE Sunset::Runtime)

if (PRODUCTION_BUILD)
    target_compile_definitions(SunsetGame
            PUBLIC
            LOCAL_PATH="./"
            PRIVATE
            SUNSET_GAME_MODULE_PATH="./$<TARGET_FILE_NAME:${SUNSET_GAME_TARGET}>"
            SUNSET_GAME_NAME="${SUNSET_GAME_TARGET}"
    )
else ()
    target_compile_definitions(SunsetGame
            PUBLIC
            LOCAL_PATH="${CMAKE_BINARY_DIR}/"
            PRIVATE
            SUNSET_GAME_MODULE_PATH=\"$<TARGET_FILE:${SUNSET_GAME_TARGET}>\"
            SUNSET_GAME_NAME="${SUNSET_GAME_TARGET}"
    )
endif ()

add_dependencies(SunsetGame Sunset::Game)
