(Engine/Audio/AudioSystem.h)
## Concept

[[AudioSystem]] est le point d'entree haut niveau pour le son.

Il est initialise et ferme par [[Application]].

```cpp
AudioSystem::Init();
AudioSystem::Update();
AudioSystem::Shutdown();
```

Il cree les [[Audio]] depuis un fichier.

```cpp
auto audio = AudioSystem::CreateAudio("music.ogg");
AudioSystem::Play2DAudio(audio, true);
```

## Buffer ou Stream

[[AudioSystem]] choisit automatiquement entre [[AudioBuffer]] et [[AudioStream]].

Les petits fichiers sont charges en memoire dans un [[AudioBuffer]].
Les gros fichiers sont lus progressivement avec [[AudioStream]].

## Sources

Le systeme garde un pool de [[AudioSource]] reutilisables.

`Play2DAudio` prend une source du pool, lui assigne l'audio, puis lance la lecture.

