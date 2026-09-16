(RenderAPI/OpenGL/Resources/OpenGLPipeline.h)
## Concept

[[OpenGLPipeline]] est l'implementation OpenGL de [[Pipeline]].

Il garde un [[RenderState]].

```cpp
RenderState m_RenderState;
```

`Bind` applique cet etat a OpenGL:

- depth test
- blending
- culling
- wireframe

