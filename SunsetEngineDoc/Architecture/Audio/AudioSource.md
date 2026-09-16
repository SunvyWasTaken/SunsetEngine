(Engine/Audio/AudioSource.h)
## Concept

[[AudioSource]] est l'objet qui joue un [[Audio]].

Il encapsule une source OpenAL.

```cpp
AudioSource source;
source.SetAudio(audio);
source.Play(loop);
```

Il peut controler la lecture:

```cpp
source.Pause();
source.Stop();
source.SetVolume(0.5f);
```

## Stream

Pour un [[AudioStream]], `Update` recycle les buffers traites par OpenAL.

```cpp
if (stream.FillBuffer(buffer, isLooping))
	alSourceQueueBuffers(source, 1, &buffer);
```

[[AudioSystem]] appelle `Update` sur toutes les sources a chaque frame.

