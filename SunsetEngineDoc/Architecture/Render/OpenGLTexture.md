(RenderAPI/OpenGL/Resources/OpenGLTexture.h)
## Concept

[[OpenGLTexture]] est l'implementation OpenGL de [[Texture]].

Elle garde l'id OpenGL, la taille et le format.

```cpp
std::uint32_t id;
std::uint32_t width, height;
TextureFormat format;
```

`Bind(slot)` attache la texture au slot demande.

