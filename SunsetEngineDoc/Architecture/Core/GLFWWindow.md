(Platform/GLFW/GLFWWindow.h)
## Concept

[[GLFWWindow]] est l'implementation concrete de [[Window]] avec GLFW.

Elle possede:

- un `GLFWwindow*`
- un [[GraphicsContext]]
- les donnees de fenetre
- le callback d'[[Event]]

```cpp
class GLFWWindow : public Window
{
	GLFWwindow* m_WindowHandle;
	std::unique_ptr<GraphicsContext> m_GraphicsContext;
	WindowData m_Data;
};
```

Elle transforme les callbacks GLFW en [[Event]] de l'engine.

