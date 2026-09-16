(Engine/GameFramework/Components/TransformComponent.h)
## Concept

[[TransformComponent]] contient la position, rotation et scale d'une [[Entity]].

```cpp
struct TransformComponent : public Component
{
	glm::vec3 Position{0.f};
	glm::quat Rotation{1.f, 0.f, 0.f, 0.f};
	glm::vec3 Scale{1.f};
};
```

Il peut produire la matrice monde.

```cpp
glm::mat4 matrix = transform.GetWorldMatrix();
```

## Reseau

Le component garde aussi des infos de synchronisation reseau.

```cpp
PeerId OwnerPeerId = 0;
bool bSyncPositionInWorld = true;
bool bBroadcastPositionInWorld = true;
```

Ces champs permettent de savoir qui possede l'objet et si sa position doit etre synchronisee.

