## Concept

Le fichier event.h possede tous les types d'evenement qu'une window peut produir.
que ce soit un evenement clavier, souris ou encore un la redimension de la fenêtre.

La class [[Event]] est une `std::variant` de tous les types d'événements.

Un [[Event]] est crée dans une implémentation Window, puis envoyé a l'application via
une Callback.
Application se charge d'envoyé l'objet event a travers les Layers.

Creation de L'event.
```cpp
// GLFWWindow.cpp
void OnKeyPressed(GLFWwindow* window, int key)
{
    Event key = KeyEvent{key};
    OnEvent(key);
}
```
Bind de la callback.
```cpp
// Application.cpp
app.Bind(std::bind(this, &Application::OnEvent, std::placeholder_1));
```
Send throw layers
```cpp
void Application::OnEvent(Event& event)
{
    for (auto& layer : layers)
        layer.OnEvent(event);
}
```

## How to Use

Dans la fonction `OnEvent` de votre [[Layer]]
vous avez plusieurs options pour récupérer un type d'evenement.

```cpp
void Layer::OnEvent(Event event)
{
    // Option 1 : tous les évent font la même action.
    std::visit([](auto&& ev)
    {
        // utiliser l'evenement.
    }, event);

    // Option 2 : faire un switch case pour variant
    std::visit(overload{
    // Implémentation de KeyEvent
    [](KeyEvent& evKey)
    {
        // Handle key event.
    },
    // Implémenter tous les autres type d'event.
    // ...
    }, event);

    // Option 3 : Si besoin que d'un event
    if (std::hold_alternative<KeyEvent>(event))
    {
        KeyEvent key = std::get<KeyEvent>(event);
        // Handle key event.
    }
    // ou
    if (KeyEvent* key = std::get_if<KeyEvent>(event))
    {
        // handle key event
    }
}
