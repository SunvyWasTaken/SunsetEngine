(Engine/Render/Resources/VertexLayout.h)
## Concept

[[VertexLayout]] decrit la structure d'un vertex.

```cpp
VertexLayout layout{
	{VertexType::Float3},
	{VertexType::Float2}
};
```

Il calcule le stride et les offsets des attributs.

```cpp
std::size_t stride = layout.GetStride();
```

[[Mesh]] utilise ce layout pour configurer les attributs GPU.

