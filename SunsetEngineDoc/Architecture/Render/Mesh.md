(Engine/Render/Resources/Mesh.h)
## Concept

[[Mesh]] represente une geometrie GPU.

Il possede un vertex buffer, un index buffer et un [[VertexLayout]].

```cpp
auto mesh = Mesh::CreateMesh(data, sizeof(Vertex), count, layout);
```

L'implementation concrete depend de la [[RenderAPI]].

```cpp
virtual void Bind() = 0;
virtual MeshHandle Get() const = 0;
```

