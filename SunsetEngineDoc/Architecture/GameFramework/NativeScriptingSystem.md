(Engine/GameFramework/System/NativeScriptingSystem.h)
## Concept

[[NativeScriptingSystem]] met a jour les [[NativeScriptComponent]] du [[World]].

Il parcourt les entities qui possedent un script natif et appelle leurs [[ScriptEntity]].

```cpp
for (auto& script : component.m_ScriptEntitys)
	script->OnUpdate(dt);
```

Il se place dans la boucle de [[World]] comme un [[IWorldSystem]].

