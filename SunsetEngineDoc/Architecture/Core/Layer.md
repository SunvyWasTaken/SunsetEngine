## Concept

Le [[Layer]] permet à [Application](lien obsidian) de ne pas connaitre tous les systems qu'il doit exécuter.
Le [[Layer]] est une sorte de module de comportement.

´´´cpp
class Layer
{
public:
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}

    virtual void OnUpdate(float dt) {}
    virtual void OnRender() {}

    virtual void OnEvent(Event& event) {}
};
´´´

Donc a ce moment je peux intégré n'importe quel autre comportement.
Un layer devient nécessaire quand un comportement devient suffisamment indépendant.

## Exemple

´´´cpp
class EditorLayer : public Layer
{
    void OnUpdate(float dt) override
    {
        // logique de l'éditeur
    }

    void OnRender() override
    {
        // rendu de l'éditeur
    }
};
´´´
