(Engine/Core/Window.h)
## Concept

[[Window]] est l'interface commune entre [[Application]] et la fenetre native.

Elle cache l'implementation concrete, par exemple [[GLFWWindow]], et expose seulement ce dont l'engine a besoin.

```cpp
class Window
{
public:
	virtual void PollEvents() = 0;
	virtual void Present() = 0;
	virtual bool ShouldClose() const = 0;
	virtual void BindEvent(const std::function<void(const Event::Type&)>& event) = 0;
};
```

[[Application]] l'utilise au debut et a la fin de chaque frame.

```cpp
m_Window->PollEvents();
// update + render
m_Window->Present();
```

La window est aussi la source principale des [[Event]].

