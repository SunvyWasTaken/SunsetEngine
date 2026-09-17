(Engine/GameFramework/World/World.h)

## Concept

[[World]] est le conteneur runtime des [[Entity]] et de leurs components.

Il encapsule un `entt::registry`.

```cpp
class World
{
	entt::registry m_Registry;
	std::vector<std::unique_ptr<IWorldSystem>> m_Systems;
};
```

Le [[World]] n'est pas directement porte par `Application`.

Il est possede par un layer, par exemple [[EditorLayer]], afin de permettre plusieurs types d'applications.

## Entity

Le world cree et detruit les [[Entity]].

```cpp
Entity CreateEntity(const std::string& name);
void DestroyEntity(const Entity& entity);
```

## Iteration

Le world expose des helpers pour parcourir les entities.

```cpp
m_World->ForEach([](const Entity& entity)
{
});
```

Et pour parcourir les entities avec certains components:

```cpp
m_World->Each<TransformComponent, SpriteRenderComponent>(
	[](const Entity& entity, TransformComponent& transform, SpriteRenderComponent& sprite)
	{
	}
);
```

## Systems

Le world possede une liste de [[IWorldSystem]].

Au constructeur, les systems engine sont enregistres.

Actuellement:

- [[NativeScriptingSystem]]

L'update appelle tous les systems:

```cpp
for (const auto& system : m_Systems)
	system->Update(dt);
```

## Input

Le world peut propager les events aux [[InputComponent]].

```cpp
void BeginInput();
bool OnEvent(const Event::Type& event);
```

En mode Play, [[EditorLayer]] appelle:

```cpp
m_World->BeginInput();
m_World->Update(dt);
```

Et forward les events:

```cpp
m_World->OnEvent(event);
```

## Clone

`World::Clone()` cree une copie en memoire du world courant.

Cette copie est utilisee par [[EditorLayer]] pour le mode Play.

Objectif:

- garder le world d'edition intact;
- lancer un runtime world modifiable;
- conserver les factories des [[NativeScriptComponent]].

Le clone copie les components supportes:

- [[TagComponent]]
- [[TransformComponent]]
- [[CameraComponent]]
- [[InputComponent]]
- [[SpriteRenderComponent]]
- [[NativeScriptComponent]]

Pour [[NativeScriptComponent]], le clone recopie les factories `InstantiateScriptEntity`, mais pas les instances runtime deja creees.

Les instances de script sont creees ensuite par [[NativeScriptingSystem]] pendant le Play.

## Serialization

Le world est serialise par [[SaveSystem]].

Le format sauvegarde:

- magic/version;
- local peer id;
- entities;
- components supportes.

La serialization actuelle ne sauvegarde pas les bindings C++ de [[NativeScriptComponent]].

Pour cette raison, le mode Play utilise `World::Clone()` et non une copie par save/load.

