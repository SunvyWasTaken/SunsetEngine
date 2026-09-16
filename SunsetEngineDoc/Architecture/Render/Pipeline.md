(Engine/Render/Resources/Pipeline.h)
## Concept

[[Pipeline]] encapsule un etat de rendu.

Il est cree depuis un [[RenderState]].

```cpp
auto pipeline = Pipeline::Create(TransparentState);
pipeline->Bind();
```

Cela permet a un [[Material]] de dire comment le GPU doit dessiner:

- depth test
- depth write
- blending
- culling
- wireframe

