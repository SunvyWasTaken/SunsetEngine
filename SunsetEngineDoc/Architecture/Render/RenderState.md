(Engine/Render/Core/RenderType.h)
## Concept

[[RenderState]] decrit l'etat de rendu utilise par un [[Pipeline]].

```cpp
struct RenderState
{
	bool depthTest = true;
	bool depthWrite = true;
	bool blending = false;
	CullMode cullMode = CullMode::Back;
	bool wireframe = false;
};
```

L'engine expose des presets.

```cpp
OpaqueState;
TransparentState;
```

Un [[Material]] peut utiliser un [[Pipeline]] cree avec l'un de ces states.

