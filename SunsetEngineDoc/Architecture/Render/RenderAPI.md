## Concept

Le [[RenderAPI]] est l'interface imlpémentée par OpenGL/Vulkan/ect.

C'est lui qui permet de parler au GPU *#JeMangeDesCarteGraphique*.

```cpp
class RenderAPI
{
public:
    virtual ~RenderAPI() = default;

    virtual void Init() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual void SetViewport(glm::ivec2 size) = 0;

    virtual void Draw(const Drawable& drawable) = 0;

    virtual std::unique_ptr<Shader>
        CreateShader(...) = 0;

    virtual std::shared_ptr<Buffer>
        CreateBuffer(...) = 0;
};
```

## OpenGL

Exemple d'implemetation OpenGL

```cpp
class OpenGLRenderAPI : public RenderAPI
{
public:
    void BeginFrame() override
    {
        glClear(...);
    }

    void Draw(const Drawable& drawable) override
    {
        glBindVertexArray(...);
        glDrawElements(...);
    }
};
```
