(Engine/Core/GraphicContext.h)
## Concept

[[GraphicsContext]] represente le contexte graphique attache a une [[Window]].

Il est responsable d'initialiser l'API native et de presenter le backbuffer.

```cpp
class GraphicsContext
{
public:
	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;
};
```

Dans l'implementation GLFW/OpenGL, la [[GLFWWindow]] possede un [[GraphicsContext]]
et appelle `SwapBuffers` pendant `Present`.

