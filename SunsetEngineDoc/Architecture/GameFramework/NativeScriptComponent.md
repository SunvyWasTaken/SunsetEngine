(Engine/GameFramework/Components/NativeScriptComponent.h)

## Concept

[[NativeScriptComponent]] attache du code C++ a une [[Entity]].

Il stocke des factories capables de creer des instances de [[ScriptEntity]].

```cpp
entity.AddComponent<NativeScriptComponent>()
	.Bind<PlayerController>();
```

## Bind

`Bind<T>()` ajoute une factory dans `InstantiateScriptEntity`.

```cpp
std::vector<std::function<ScriptEntity*()>> InstantiateScriptEntity;
```

Ces factories ne sont pas serialisees dans les fichiers de world.

Elles sont conservees pendant le Play grace a `World::Clone()`.

## Start

`Start(World* world, const Entity& entity)` instancie les scripts.

Pour chaque factory:

- cree une instance de [[ScriptEntity]];
- lui assigne l'entity proprietaire;
- appelle `OnBeginPlay`.

```cpp
m_ScriptEntitys.emplace_back(factory());
m_ScriptEntitys.back()->m_Entity = entity;
m_ScriptEntitys.back()->OnBeginPlay();
```

## Update

[[NativeScriptingSystem]] appelle `OnUpdate(dt)` sur chaque script instancie.

Si aucun script n'est encore instancie, le system appelle d'abord `Start`.

## Stop

`Stop()` appelle `OnEndPlay()` sur chaque script runtime, puis vide la liste d'instances.

```cpp
for (const auto& scriptEntity : m_ScriptEntitys)
	scriptEntity->OnEndPlay();

m_ScriptEntitys.clear();
```

[[EditorLayer]] appelle `Stop()` sur les scripts du runtime world quand le mode Play est arrete.

## Cycle de vie en Play Mode

1. [[EditorLayer]] clone le world d'edition.
2. Le runtime world devient le world actif.
3. `World::Update(dt)` lance [[NativeScriptingSystem]].
4. Les scripts recoivent `OnBeginPlay`.
5. Les scripts recoivent `OnUpdate(dt)` chaque frame.
6. Au Stop, les scripts recoivent `OnEndPlay`.
7. L'editeur restaure le world d'edition original.

