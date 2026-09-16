(Engine/Render/Resources/Buffer.h)
## Concept

[[Buffer]] est l'abstraction d'un buffer GPU.

```cpp
auto buffer = Buffer::Create(BufferType::Vertex);
buffer->SetData(vertices, sizeof(Vertex), count);
```

Il peut etre:

```cpp
enum class BufferType
{
	Vertex,
	Index,
	Uniform
};
```

Le buffer expose aussi un [[RenderHandles]] pour l'API concrete.

