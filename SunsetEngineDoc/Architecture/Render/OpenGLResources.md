(RenderAPI/OpenGL/Resources)
## Concept

[[OpenGLResources]] regroupe les implementations OpenGL des ressources de rendu.

Chaque classe implemente une interface de `Engine/Render/Resources`.

```cpp
OpenGLBuffer      -> [[Buffer]]
OpenGLMesh        -> [[Mesh]]
OpenGLShader      -> [[Shader]]
OpenGLTexture     -> [[Texture]]
OpenGLPipeline    -> [[Pipeline]]
OpenGLRenderTarget -> [[RenderTarget]]
```

Ces classes possedent les ids OpenGL et traduisent les appels de l'engine en appels API natifs.

```cpp
BufferHandle id;
ShaderHandle id;
std::uint32_t textureId;
```

Le reste de l'engine manipule les interfaces haut niveau.

