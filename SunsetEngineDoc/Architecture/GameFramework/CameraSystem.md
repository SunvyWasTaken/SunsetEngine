(Engine/GameFramework/System/CameraSystem.h)
## Concept

[[CameraSystem]] est un [[IWorldSystem]] dedie aux [[CameraComponent]].

Il permet de synchroniser les cameras avec les entities du [[World]].

La camera primaire est ensuite utilisee par [[BuildRenderScene]] ou le [[Renderer]]
pour dessiner la [[RenderScene]].

