(Engine/Network/NetworkType.h)
## Concept

[[NetworkType]] regroupe les types partages par le reseau.

On y trouve les ids et les structures de base:

```cpp
using PeerId = uint32_t;
using ChannelId = uint8_t;
```

Ces types servent a garder une API reseau stable entre [[NetworkService]],
[[INetworkTransport]] et les messages gameplay.

```cpp
struct EndPoint
{
	std::string Address = "127.0.0.1";
	uint16_t Port = 7777;
};

enum class DeliveryType
{
	Reliable,
	Unreliable
};
```

`Packet` contient le peer source, le channel et le payload brut.
