(Platform/GLFW/GLFWWindow.h)
## Concept

[[GLFWWindow]] est l'implementation concrete de [[Window]] avec GLFW.

Elle possede:

- un `GLFWwindow*`
- les donnees de fenetre
- le callback d'[[Event]]

```cpp
class GLFWWindow : public Window
{
	GLFWwindow* m_WindowHandle;
	WindowData m_Data;
};
```

Elle transforme les callbacks GLFW en [[Event]] de l'engine.
