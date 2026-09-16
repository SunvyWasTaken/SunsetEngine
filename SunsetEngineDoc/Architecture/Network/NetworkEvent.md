(Engine/Network/NetworkEvent.h)
## Concept

[[NetworkEvent]] est le format commun retourne par un [[INetworkTransport]].

```cpp
using Type = std::variant<
	PeerConnected,
	PeerDisconnected,
	PacketReceived>;
```

[[NetworkService]] lit ces events dans `Update`.

- `PeerConnected` previent qu'un peer est disponible
- `PeerDisconnected` previent qu'un peer a quitte
- `PacketReceived` contient un packet brut a dispatcher

