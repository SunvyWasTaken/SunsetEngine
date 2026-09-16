(RenderAPI/OpenGL/OpenGLDrawQueue.h)
## Concept

[[OpenGLDrawQueue]] stocke les commandes de draw avant de les executer.

```cpp
queue.UseCamera(camera);
queue.Submit(drawable, model);
queue.Flush();
```

Elle garde les matrices de camera pour la frame.

```cpp
struct FrameData
{
	glm::mat4 view;
	glm::mat4 projection;
};
```

`Flush` trie puis dessine les commandes avec les ressources OpenGL.

