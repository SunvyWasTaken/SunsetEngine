(Engine/Render/Core/RenderHandles.h)
## Concept

[[RenderHandles]] regroupe les handles de ressources GPU.

Tous les handles heritent de `ResourceHandle`.

```cpp
struct BufferHandle : ResourceHandle {};
struct MeshHandle : ResourceHandle {};
struct ShaderHandle : ResourceHandle {};
```

Un handle vaut `false` quand son id est `0`.

```cpp
if (shaderHandle)
{
	// resource valide
}
```

Cela donne une representation legere des ressources natives sans exposer directement l'API graphique.

