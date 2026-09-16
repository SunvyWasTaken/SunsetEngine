(Engine/Audio/AudioBuffer.h)
## Concept

[[AudioBuffer]] represente un son entierement charge en memoire.

Il possede un buffer OpenAL et expose son id.

```cpp
AudioBuffer buffer;
buffer.LoadFile("hit.wav");
auto id = buffer.GetBufferID();
```

Il est adapte aux sons courts comme les impacts, UI sounds ou effets repetes.

## Donnees

`SetData` permet de remplir le buffer avec des samples deja decodes.

```cpp
buffer.SetData(channels, sampleRate, samples, sizeInBytes);
```

