(Engine/Audio/AudioStream.h)
## Concept

[[AudioStream]] represente un son lu progressivement depuis le disque.

Il garde plusieurs [[AudioBuffer]] internes qui sont remplis au fur et a mesure.

```cpp
AudioStream stream;
stream.LoadFile("music.ogg");
stream.FillBuffer(bufferId, loop);
```

[[AudioSource]] depile les buffers deja joues, appelle `FillBuffer`,
puis remet le buffer dans la queue OpenAL.

## Usage

Il est surtout fait pour les musiques et sons longs.

