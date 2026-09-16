(Engine/Render/Resources/Shader.h)
## Concept

[[Shader]] abstrait un programme GPU.

```cpp
auto shader = Shader::CreateShader(vertexSource, fragmentSource);
shader->Bind();
```

Il expose les setters d'uniforms utilises par [[Material]].

```cpp
shader->SetFloat("u_Time", time);
shader->SetMat4("u_ViewProjection", vp);
```

L'implementation concrete est fournie par la [[RenderAPI]].

