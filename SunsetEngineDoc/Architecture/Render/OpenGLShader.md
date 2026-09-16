(RenderAPI/OpenGL/Resources/OpenGLShader.h)
## Concept

[[OpenGLShader]] est l'implementation OpenGL de [[Shader]].

Il compile le vertex shader et le fragment shader,
puis expose les setters d'uniforms.

```cpp
shader.SetMat4("u_ViewProjection", vp);
shader.SetVec4("u_Color", color);
```

Il garde un [[RenderHandles]] de type shader.

