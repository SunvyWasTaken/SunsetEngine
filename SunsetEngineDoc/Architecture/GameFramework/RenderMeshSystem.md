(Engine/GameFramework/System/RenderMeshSystem.h)
## Concept

[[RenderMeshSystem]] est un [[IWorldSystem]] lie au rendu des objets du [[World]].

Son role est de traiter les entities possedant des components de rendu,
comme [[SpriteRenderComponent]] ou un futur mesh component.

Dans l'architecture actuelle, la construction concrete de la scene de rendu passe surtout par [[BuildRenderScene]].

