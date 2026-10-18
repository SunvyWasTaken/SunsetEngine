(Engine/Core/Layer.h)

## Concept

Un [[Layer]] ajoute un comportement à [[Application]]. La boucle appelle `OnUpdate(float)` pour la logique, `OnDraw()` pour les interfaces graphiques et `OnEvent(const Event::Type&)` pour les événements.

L'éditeur implémente `OnDraw()` pour afficher son interface ImGui. Le moteur ne contient actuellement aucun rendu de scène.
