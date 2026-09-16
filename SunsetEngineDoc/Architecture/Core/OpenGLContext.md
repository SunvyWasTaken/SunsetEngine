(Platform/GLFW/OpenGLContext.h)
## Concept

[[OpenGLContext]] est l'implementation OpenGL de [[GraphicsContext]].

Il est attache a une `GLFWwindow`.

```cpp
class OpenGLContext final : public GraphicsContext
{
	GLFWwindow* _window;
};
```

`Init` rend le contexte courant et initialise OpenGL.

`SwapBuffers` presente l'image de la [[Window]].

