# SunsetEngine

> SunsetEngine is a C++20 game-engine library built around OpenGL rendering, a layer-based application loop, an Entity Component System powered by EnTT, UI widgets, logging/profiling utilities, and an ENet-based networking layer. The project currently builds a static library named `SunsetEngine` that can be linked by a game or sandbox executable.

---

## Features

- **Application core**: window settings, a main run loop, layer stack management, deferred layer loading/clearing, and event dispatch.
- **Rendering**: OpenGL-backed renderer, frame commands, cameras, shaders, materials, textures, meshes, drawables, render states, and vertex/index buffer abstractions.
- **Game framework**: worlds, entities, controllers, and reusable components such as transforms and camera components.
- **Input**: keyboard and mouse events, named input actions loaded from JSON, local input sources, and network input sources.
- **Networking**: ENet transport, host/join workflow, typed message sending/broadcasting, peer connection callbacks, and packet dispatch by channel.
- **Slate UI**: basic immediate-style UI objects including square panels, images, and horizontal boxes.
- **Utilities**: logging through spdlog, on-screen debug text, profiling scopes, math helpers, and a reusable debug cube primitive.
- **Third-party noise**: FastNoiseSIMD is included in `Thirdparty/FastNoiseSIMD` and built with the engine.

---

## Repository layout

```text
.
├── CMakeLists.txt                  # CMake definition for the SunsetEngine static library
├── vcpkg.json                      # vcpkg manifest listing external dependencies
├── Engine/
│   ├── SunsetEngine.h              # Umbrella include for engine consumers
│   ├── BaseObject/                 # Built-in drawable helpers such as debug cubes
│   ├── Core/                       # Application, layers, layer stack, input, settings
│   ├── GameFramework/              # World/entity/controller logic and components
│   ├── Log/                        # Logger and on-screen print helpers
│   ├── Math/                       # Math primitives such as AABB/frustum support
│   ├── Network/                    # Network service, events, transport interface, ENet backend
│   ├── Render/                     # Renderer, camera, shaders, materials, buffers, textures, meshes
│   ├── Slate/                      # UI widgets and layout primitives
│   └── Utility/                    # Profiling and utility functions
├── Shaders/                        # Built-in GLSL shaders used by engine drawables and UI
└── Thirdparty/FastNoiseSIMD/       # Vendored FastNoiseSIMD library
```

---

## Requirements

- A C++20 compiler.
- CMake 3.28 or newer.
- vcpkg, recommended for dependency installation through the included manifest.
- OpenGL-capable graphics drivers.

The vcpkg manifest declares these external packages:

- glad
- glfw3
- glm
- opengl
- stb
- nlohmann-json
- spdlog
- imgui with GLFW and OpenGL 3 bindings
- enet
- entt

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

### Headless mode

Set `ApplicationSetting::Headless` to `true` for a server-style process that should not create a GLFW window, OpenGL context, ImGui context, or run draw frames. The application loop still updates layers and automatically ticks `NetworkService` when it has been initialized, so a headless layer can host or join a network session without depending on rendering.

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

For Minecraft-like first-person hands or held items, submit the normal world drawables as usual, then submit the hand/item drawable with `RenderState::AfterWorldOverlay()`. This keeps the engine's current `RenderCommande::Submit(...)` flow while letting the game opt into a second render layer for meshes that should not clip into walls.

```cpp
Sunset::Drawable hand = CreateHandDrawable();
hand.m_RenderState = Sunset::RenderState::AfterWorldOverlay();

Sunset::RenderCommande::Submit(hand, handTransform);
```

`AfterWorldOverlay()` sets the drawable layer to `RenderLayer::Overlay` and disables depth test/write for that drawable. Because `RenderCommande` already sorts overlay drawables after world drawables, the world depth buffer can no longer hide the hand/item. If you want an overlay mesh that still participates in depth testing, keep using a custom `RenderState` and set only the fields you need.

---

## Runtime data

SunsetEngine defines a `SAVE_PATH` compile definition that points to `${CMAKE_SOURCE_DIR}/Save/`. The input system initializes from `Input.json` inside that folder, so applications should provide a file such as:

```text
Save/Input.json
```

Shader paths are compiled into the engine through `ENGINE_SHADERS_PATH` and point to the repository `Shaders/` directory.

---

## Networking overview

The networking layer is exposed through `Sunset::NetworkService`. It supports hosting, joining, reliable/unreliable delivery modes, peer connection/disconnection handlers, and trivially copyable typed messages.

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
```

Remember to call `NetworkService::Shutdown()` when your application is done if you initialize the service manually.

---

## Development notes

- The root [`CMakeLists.txt`](CMakeLists.txt) currently uses `file(GLOB_RECURSE ...)` to collect engine sources.
- [`Engine/SunsetPCH.h`](Engine/SunsetPCH.h) is configured as a public precompiled header for the engine target.
- `Thirdparty/FastNoiseSIMD` is built as a subdirectory and linked publicly.
- The engine target exposes `Engine/` as a public include directory, so headers can be included relative to that folder.

---

## License

This repository includes a `LICENSE` file. Review it before distributing or reusing the engine.
