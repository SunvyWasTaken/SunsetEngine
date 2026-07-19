

if (SUNSET_USE_OPENGL)
    add_subdirectory(OpenGL)
endif ()
if (SUNSET_USE_VULKAN)
    add_subdirectory(Vulkan)
endif ()
