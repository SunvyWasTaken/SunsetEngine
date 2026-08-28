
(Engine/Core/Application.h)
## Concept

[[Application]] est littéralement le coeur du projet.

Il possède un [[LayerStack]] qu'il met à jour et dessine à chaque frame.

```cpp
while(IsAppRunning)
{
	// ...
	for (const auto& layer : m_LayerStack)
		layer->OnUpdate(deltatime);

	// ...
	
	for (auto layer = m_LayerStack.end(); layer != m_LayerStack.begin())
		(*--layer)->OnDraw();
		
	// ...
}
```

C'est aussi lui qui possède le [[World]] actuel.

Il est aussi charger de call [[RenderCommand]]. C'est à dire les fonction BeginFrame(), EndFrame() et Shutdown().

*Je pense que je dois changer cela. Parce que cela veut dire que litteralement j'ai mon objet Render qui flot dans le vide. Si par pur hasard j'oublie de call shutdown il sera la tous seul. :/*
