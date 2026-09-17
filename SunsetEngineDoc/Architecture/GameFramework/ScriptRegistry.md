(Engine/GameFramework/World/ScriptRegistry.h)

## Concept

[[ScriptRegistry]] est le registre global des scripts natifs disponibles dans le moteur.

Il sert a associer un nom de script a une factory C++ capable de creer une instance de [[ScriptEntity]].

```cpp
struct ScriptRegistryEntry
{
	std::string Name;
	std::function<ScriptEntity*()> Instantiate;
};
```

Le registre ne stocke pas les instances runtime des scripts. Il stocke uniquement:

- le nom public du script;
- la fonction qui sait construire ce script.

Les instances sont creees plus tard par [[NativeScriptComponent]] au moment du `Start`.

## Pourquoi il existe

`NativeScriptComponent::Bind<T>()` permet d'attacher un script directement en C++, mais la factory ajoutee par `Bind` n'est pas serialisee telle quelle.

Le registre resout ce probleme avec un nom stable:

1. un module de jeu enregistre ses scripts au demarrage;
2. l'editeur liste les scripts enregistres;
3. l'utilisateur attache un script a une entity;
4. le [[NativeScriptComponent]] sauvegarde seulement les noms;
5. au chargement, les noms sont resolus avec [[ScriptRegistry]] pour reconstruire les factories.

Le registre fait donc le pont entre le code C++ et les donnees sauvegardees du world.

## Stockage interne

Les scripts sont stockes dans un vecteur statique interne au `.cpp`.

```cpp
std::vector<Sunset::ScriptRegistryEntry> s_Scripts;
```

Ce stockage est global au process. Il n'appartient pas a un [[World]], a une [[Entity]] ou a l'editeur.

## Register

`Register<T>(name)` enregistre un type derive de [[ScriptEntity]].

```cpp
ScriptRegistry::Register<PlayerController>("PlayerController");
```

La version template cree automatiquement une factory qui retourne `new T()`.

```cpp
return new T();
```

Il existe aussi une version bas niveau:

```cpp
static void Register(std::string name, std::function<ScriptEntity*()> instantiate);
```

Elle permet de fournir une factory custom.

```cpp
ScriptRegistry::Register("EnemyAI", []() -> ScriptEntity*
{
	return new EnemyAI(/* args custom */);
});
```

Si un script avec le meme nom existe deja, le registre remplace sa factory.

## GetScripts

`GetScripts()` retourne la liste complete des scripts enregistres.

```cpp
const auto& scripts = ScriptRegistry::GetScripts();
```

L'editeur l'utilise dans [[WorldHierarchyPanel]] pour afficher le menu "Add Native Script".

Si aucun script n'est enregistre, l'editeur affiche:

```text
No native scripts registered
```

## Find

`Find(name)` cherche une entree par son nom.

```cpp
const ScriptRegistryEntry* entry = ScriptRegistry::Find("PlayerController");
```

La fonction retourne:

- un pointeur vers l'entree si le nom existe;
- `nullptr` si aucun script ne correspond.

## AddScriptTo

`AddScriptTo(component, name)` ajoute un script enregistre a un [[NativeScriptComponent]].

```cpp
ScriptRegistry::AddScriptTo(nativeScriptComponent, "PlayerController");
```

La fonction:

1. cherche le script avec `Find`;
2. retourne `false` si le nom n'existe pas;
3. appelle `NativeScriptComponent::AddRegisteredScript`;
4. retourne `true` si l'ajout a reussi.

`NativeScriptComponent::AddRegisteredScript` evite les doublons avec `RegisteredScriptNames`.

## Interaction avec NativeScriptComponent

Quand un script est ajoute depuis le registre, le component conserve deux informations:

```cpp
std::vector<std::string> RegisteredScriptNames;
std::vector<std::function<ScriptEntity*()>> InstantiateScriptEntity;
```

`RegisteredScriptNames` sert a la sauvegarde.

`InstantiateScriptEntity` sert au runtime pour creer les instances de [[ScriptEntity]].

Au lancement du gameplay, [[NativeScriptingSystem]] provoque le `Start` du component. Le component parcourt alors ses factories, cree les scripts et appelle `OnBeginPlay`.

## Serialization

Le [[NativeScriptComponent]] ne serialise pas les factories C++.

En sauvegarde, il ecrit uniquement les noms:

```cpp
std::vector<std::string> scriptNames = component.GetRegisteredScriptNames();
archive(scriptNames);
```

En chargement, il:

1. lit la liste de noms;
2. stoppe les scripts runtime existants;
3. vide les noms et les factories;
4. appelle `ScriptRegistry::AddScriptTo` pour chaque nom.

```cpp
for (const auto& scriptName : scriptNames)
	ScriptRegistry::AddScriptTo(component, scriptName);
```

Si un nom sauvegarde n'est pas present dans le registre au moment du chargement, ce script ne peut pas etre reconstruit.

## Limites actuelles

- Les scripts doivent etre enregistres avant que l'editeur les liste ou qu'un world les charge.
- Les noms doivent rester stables si des worlds deja sauvegardes les utilisent.
- Le registre est global et ne gere pas de namespace par module.
- La factory template suppose un constructeur par defaut.
- La version custom permet des arguments, mais ces arguments ne sont pas serialises automatiquement.

# How To Use

## 1. Creer un script

Creer une classe qui herite de [[ScriptEntity]].

```cpp
#include "GameFramework/World/ScriptEntity.h"

class PlayerController final : public Sunset::ScriptEntity
{
public:
	void OnBeginPlay() override;
	void OnUpdate(float dt) override;
	void OnEndPlay() override;
};
```

## 2. Enregistrer le script

Enregistrer le script au demarrage du module de jeu, avant l'ouverture du world ou de l'editeur.

```cpp
#include "GameFramework/World/ScriptRegistry.h"

void RegisterGameScripts()
{
	Sunset::ScriptRegistry::Register<PlayerController>("PlayerController");
}
```

Le nom `"PlayerController"` est le nom qui sera affiche dans l'editeur et sauvegarde dans le world.

## 3. Ajouter le script depuis l'editeur

Dans le [[WorldHierarchyPanel]]:

1. clic droit sur une entity;
2. `AddComponent`;
3. `Native Script Component`;
4. choisir le script enregistre.

L'editeur ajoute automatiquement un [[NativeScriptComponent]] si l'entity n'en possede pas encore.

## 4. Ajouter le script en C++

Pour ajouter un script enregistre depuis le code:

```cpp
auto& component = entity.AddComponent<Sunset::NativeScriptComponent>();
Sunset::ScriptRegistry::AddScriptTo(component, "PlayerController");
```

Verifier le retour si le nom peut etre invalide.

```cpp
if (!Sunset::ScriptRegistry::AddScriptTo(component, "PlayerController"))
{
	LOG("Game", warn, "PlayerController is not registered")
}
```

## 5. Garder les noms stables

Quand un world a ete sauvegarde avec un script, eviter de renommer son entree dans le registre.

```cpp
Sunset::ScriptRegistry::Register<PlayerController>("PlayerController");
```

Si le nom change, les anciennes sauvegardes ne pourront plus retrouver la factory correspondante.
