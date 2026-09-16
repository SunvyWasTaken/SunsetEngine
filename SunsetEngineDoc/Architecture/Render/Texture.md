(Engine/Render/Resources/Texture.h)
## Concept

[[Texture]] represente une texture GPU.

Elle est creee depuis une description.

```cpp
TextureDescription desc;
desc.width = width;
desc.height = height;
desc.format = TextureFormat::RGBA;
desc.data = pixels;

auto texture = Texture::Create(desc);
```

Elle peut ensuite etre bind sur un slot.

```cpp
texture->Bind(0);
```

