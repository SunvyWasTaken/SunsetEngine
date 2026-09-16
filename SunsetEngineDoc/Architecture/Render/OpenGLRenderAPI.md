(RenderAPI/OpenGL/OpenGLRenderAPI.h)
## Concept

[[OpenGLRenderAPI]] est l'implementation OpenGL de [[RenderAPI]].

Elle cree les ressources concretes:

- [[OpenGLShader]]
- [[OpenGLBuffer]]
- [[OpenGLTexture]]
- [[OpenGLPipeline]]
- [[OpenGLMesh]]
- [[OpenGLRenderTarget]]

```cpp
std::unique_ptr<RenderAPI> api = std::make_unique<OpenGLRenderAPI>();
app.SetRenderAPI(std::move(api));
```

[[Renderer]] parle a l'interface [[RenderAPI]], pas directement a OpenGL.

## Frame

[[OpenGLRenderAPI]] gere les appels de frame:

```cpp
BeginFrame();
BeginScene(camera);
Submit(drawable, transform);
EndScene();
EndFrame();
```

