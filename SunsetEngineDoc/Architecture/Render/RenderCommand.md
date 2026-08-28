#Idea
## Concept

Render encapsule les commandes bas niveaux.

```cpp
// RenderCommand.h
class RenderCommand
{
public:
	static void BeginFrame();
	static void EndFrame();
	
	static void Submit(const Drawable& obj, const glm::mat4& model);
	// ect...
}
```

Donc dans les fonction call du render command on va call les fonction depuis le [[RenderAPI]].

```cpp
// RenderCommand.cpp
void RenderCommand::BeginFrame()
{
	RenderAPI().BeginFrame();
}

void RenderCommand::EndFrame()
{
	RenderAPI().EndFrame();
}

void RenderCommand::Submit(const Drawable& obj, const glm::mat4& model)
{
	// Ici il va y avoir du changement.
	// Lorsqu'il y a un submit il la rajoute dans une [[DrawQueue]]
	
	// pour le moment je fais
	RenderAPI().Submit(obj, model);
	// ce qui envoie directement le tous dans une grande draw commande qui va tous render au moment du flush.
}
```

Donc le submit va rajouté va crée une cmd. pour ajouté le Drawable dans la bonne [[DrawQueue]]. Mais comment?

je pensais faire un truc du genre
```cpp
void RenderCommand::Submit(const Drawable& obj, const glm::mat4& model)
{
	if (obj.Opaque)
		RenderAPI().Submit(OpaqueQueue, obj);
	
	if (obj.Transparent)
		RenderAPI().Submit(TransparentQueue, obj);
		
	// ...
}
```

Je trouve la solution pas encore très viable mais pour le moment je ne vois que ça concrètement.