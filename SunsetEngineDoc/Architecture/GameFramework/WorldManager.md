(Engine/GameFramework/World/WorldManager.h)
## Concept

[[WorldManager]] garde le [[World]] actif.

```cpp
auto world = manager.CreateWorld();
manager.SetActiveWorld(world);
```

Il sert de point de passage quand une application ou un editor doit changer de monde.

```cpp
World* world = manager.GetActiveWorldRaw();
```

