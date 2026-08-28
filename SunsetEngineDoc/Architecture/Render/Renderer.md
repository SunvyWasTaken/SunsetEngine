## Concept

[[Renderer]] est le Haut niveau de l'interface.

Il permet de dire comment je veux render un [[RenderScene]] en appelant les fonction de [[RenderCommand]].

```cpp
class Renderer
{
public:

    void RenderScene(const RenderScene& scene)
    {
        RenderCommand::BeginFrame();

        RenderCommand::UseCamera(scene.GetCamera());

        for (const auto& object : scene.GetObjects())
        {
            RenderCommand::Submit(
                object.drawable,
                object.transform
            );
        }

        RenderCommand::EndFrame();
    }
};
```

