(RenderAPI/OpenGL/Resources/OpenGLBuffer.h)
## Concept

[[OpenGLBuffer]] est l'implementation OpenGL de [[Buffer]].

Il possede un [[RenderHandles]] et le type de buffer.

```cpp
BufferHandle id;
BufferType m_Type;
```

`SetData` upload les donnees dans le buffer OpenGL.

