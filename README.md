# SunsetEngine

SunsetEngine is a C++20 static game-engine library built around an OpenGL renderer, a layer-based application loop, an EnTT-powered Entity Component System, SRmGUI UI widgets, logging/profiling helpers, and an ENet networking layer. It is intended to be added to a game or sandbox project with CMake and linked as the `SunsetEngine` target.

---

## Features

- **Application core**: window settings, main loop, layer and overlay stack management, deferred layer loading/clearing, event dispatch, and optional headless execution.
- **Rendering**: OpenGL renderer, frame commands, framebuffer targets, cameras, shaders, materials, textures, meshes, drawables, render states, and vertex/index/uniform buffer abstractions.
- **Game framework**: worlds, entities, controllers, and reusable components such as transforms and cameras.
- **Input**: keyboard and mouse events, named input actions loaded from JSON, local input sources, and network input sources.
- **Networking**: ENet transport, host/join workflow, reliable or unreliable delivery, typed message send/broadcast helpers, peer connection callbacks, and packet dispatch by channel.
- **UI**: SRmGUI retained-mode widgets such as panels, buttons, text, images, overlays, grids, vertical boxes, and horizontal boxes, backed by an OpenGL implementation.
- **Utilities**: spdlog logging, on-screen debug text, profiling scopes, math helpers, and a reusable debug cube primitive.
- **Third-party noise**: FastNoiseSIMD is vendored in `Thirdparty/FastNoiseSIMD` and built with the engine.

---

## Repository layout

```text
.
├── CMakeLists.txt                  # CMake definition for the SunsetEngine static library
├── vcpkg.json                      # vcpkg manifest listing external dependencies
├── Engine/
│   ├── SunsetEngine.h              # Umbrella include for common engine APIs
│   ├── BaseObject/                 # Built-in drawable helpers such as debug cubes
│   ├── Core/                       # Application, layers, layer stack, input, settings
│   ├── GameFramework/              # World/entity/controller logic and components
│   ├── Log/                        # Logger and on-screen print helpers
│   ├── Math/                       # Math primitives such as AABB support
│   ├── Network/                    # Network service, events, transport interface, ENet backend
│   ├── Render/                     # Renderer, camera, shaders, materials, buffers, textures, meshes
│   └── Utility/                    # Profiling and utility functions
├── Shaders/                        # Built-in GLSL shaders used by engine drawables and UI
├── Thirdparty/FastNoiseSIMD/       # Vendored FastNoiseSIMD library
└── Thirdparty/SRmGUI/              # Retained-mode GUI library used by the engine
```

---

## Requirements

- A C++23 compiler.
- CMake 3.28 or newer.
- vcpkg, recommended for dependency installation through the included manifest.
- OpenGL-capable graphics drivers for non-headless applications.

The vcpkg manifest declares these external packages:

- assimp
- enet
- entt
- glad
- glfw3
- glm
- imgui with GLFW, OpenGL 3, and docking bindings
- nlohmann-json
- spdlog

---

## Building

The project is configured as a static library. The most convenient setup is to use vcpkg manifest mode with CMake.

```bash
git clone <repository-url>
cd SunsetEngine

cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build build
```

If vcpkg is already integrated globally or your environment exposes the required packages through another package manager, you can configure without the toolchain file:

```bash
cmake -S . -B build
cmake --build build
```

For production-style runtime paths, configure with `-DPRODUCTION_BUILD=ON`. In that mode, `SAVE_PATH` resolves to `./Save/` and `ENGINE_SHADERS_PATH` resolves to `./Shaders/`. Otherwise, the development build uses the source tree paths.

---

## Using SunsetEngine from another CMake project

Add the engine as a subdirectory and link the generated static library:

```cmake
add_subdirectory(path/to/SunsetEngine)

target_link_libraries(MyGame PRIVATE SunsetEngine)
```

Then include the umbrella header from your game code:

```cpp
#include <SunsetEngine.h>

class GameLayer final : public Sunset::Layer
{
public:
    void OnUpdate(float dt) override
    {
        // Update gameplay here.
    }

    void OnDraw() override
    {
        // Submit draw commands here.
    }
};

int main()
{
    Sunset::ApplicationSetting settings;
    settings.WindowSize = {1280, 720};
    settings.WindowTitle = "Sunset Game";

    Sunset::Application app(settings);
    app.PushLayer<GameLayer>();
    app.Run();
}
```

---

## Headless mode

Set `ApplicationSetting::Headless` to `true` for a server-style process that should not create a GLFW window, OpenGL context, ImGui context, or render frames. The application loop still updates layers and automatically ticks `NetworkService` when it has been initialized, so a headless layer can host or join a network session without depending on rendering.

```cpp
Sunset::ApplicationSetting settings;
settings.Headless = true;
settings.HeadlessTickRate = 60.0f;

Sunset::Application app(settings);
app.PushLayer<ServerLayer>();
app.Run();
```

---

## Rendering first-person items after the world

For Minecraft-like first-person hands or held items, submit the normal world drawables as usual, then submit the hand/item drawable with `RenderState::AfterWorldOverlay()`. This keeps the current `RenderCommande::Submit(...)` flow while letting the game opt into a second render layer for meshes that should not clip into walls.

```cpp
Sunset::Drawable hand = CreateHandDrawable();
hand.m_RenderState = Sunset::RenderState::AfterWorldOverlay();

Sunset::RenderCommande::Submit(hand, handTransform);
```

`AfterWorldOverlay()` sets the drawable layer to `RenderLayer::Overlay` and disables depth test/write for that drawable. Because `RenderCommande` sorts overlay drawables after world drawables, the world depth buffer can no longer hide the hand/item. If you want an overlay mesh that still participates in depth testing, use a custom `RenderState` and set only the fields you need.

---

## Runtime data

SunsetEngine uses compile definitions for runtime data paths:

- `SAVE_PATH`: points to `./Save/` when `PRODUCTION_BUILD` is enabled, otherwise to `${CMAKE_SOURCE_DIR}/Save/`.
- `ENGINE_SHADERS_PATH`: points to `./Shaders/` when `PRODUCTION_BUILD` is enabled, otherwise to this repository's `Shaders/` directory.

The input system initializes from `Input.json` inside `SAVE_PATH` for non-headless applications, so applications should provide a file such as:

```text
Save/Input.json
```

---

## Networking overview

The networking layer is exposed through `Sunset::NetworkService`. It supports hosting, joining, reliable/unreliable delivery modes, peer connection/disconnection handlers, and trivially copyable typed messages. Message channels can be declared with `T::ChannelId` or registered explicitly with `RegisterMessage<T>(channel)` before sending or receiving that type.

```cpp
struct PlayerMove
{
    static constexpr Sunset::ChannelId ChannelId = 1;
    float X;
    float Y;
    float Z;
};

Sunset::NetworkService::Init();
auto& network = Sunset::NetworkService::Get();

network.Host(7777, 8);
network.RegisterHandler<PlayerMove>([](Sunset::PeerId peer, const PlayerMove& move) {
    // Handle movement from peer.
});

PlayerMove move{.X = 1.0f, .Y = 0.0f, .Z = 0.0f};
network.Broadcast(move);

Sunset::NetworkService::Shutdown();
```

Call `NetworkService::Shutdown()` when your application is done if you initialize the service manually.

---

## Development notes

- The root `CMakeLists.txt` uses `file(GLOB_RECURSE ...)` to collect engine sources.
- `Engine/SunsetPCH.h` is configured as a public precompiled header for the engine target.
- `Thirdparty/FastNoiseSIMD` and `Thirdparty/SRmGUI` are built as subdirectories and linked publicly.
- The engine target exposes `Engine/` as a public include directory, so headers can be included relative to that folder.

---

## License

This repository includes a `LICENSE` file. Review it before distributing or reusing the engine.
