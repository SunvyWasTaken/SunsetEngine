
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

Application est aussi responsable de la vie de [[Window]] et [[Renderer]].
