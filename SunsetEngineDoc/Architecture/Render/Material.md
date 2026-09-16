(Engine/Render/Resources/Material.h)
## Concept

[[Material]] regroupe ce dont un [[Drawable]] a besoin pour etre rendu.

Il possede:

- un [[Pipeline]]
- un [[Shader]]
- des [[Texture]]
- des uniforms

```cpp
material.LoadShader("SpriteShader.vert", "SpriteShader.frag");
material.Set("u_Color", glm::vec4(1.f));
```

`Bind` active le pipeline, le shader et les textures.

`UniformBind` envoie les uniforms au shader.

