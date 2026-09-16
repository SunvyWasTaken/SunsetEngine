(Engine/Core/WindowSetting.h)
## Concept

[[WindowSetting]] contient la configuration de lancement de [[Application]].

```cpp
struct WindowSetting
{
	std::string_view WindowTitle{"SunsetEngine"};
	glm::ivec2 WindowSize{1920, 1080};
	bool vSync{true};
	bool Headless{false};
	float HeadlessTickRate{60.0f};
};
```

Le mode `Headless` permet de faire tourner l'engine sans [[Window]] visible.

Dans ce mode, [[Application]] saute le rendu et limite la boucle avec `HeadlessTickRate`.

