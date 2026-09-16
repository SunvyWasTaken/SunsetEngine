(Engine/Core/GameModule.h)
## Concept

[[GameModule]] est l'interface utilisee pour charger du code de jeu dynamiquement.

Un module doit implementer `IGameModule`.

```cpp
struct IGameModule
{
	virtual void Load(Application& app) = 0;
	virtual void Unload() = 0;
};
```

Le macro `CREATE_MODULE` exporte deux fonctions C:

```cpp
SunsetCreateGameModule();
SunsetDestroyGameModule(module);
```

Cela permet au [[GameModuleLoader]] de creer et detruire le module sans connaitre sa classe concrete.

