(Engine/Audio/Audio.h)
## Concept

[[Audio]] est un variant entre [[AudioBuffer]] et [[AudioStream]].

```cpp
using Audio = std::variant<AudioBuffer, AudioStream>;
```

Cela permet a [[AudioSource]] de jouer un son court ou un stream avec la meme API.

```cpp
std::shared_ptr<Audio> audio = AudioSystem::CreateAudio(path);
source.SetAudio(audio);
source.Play();
```

