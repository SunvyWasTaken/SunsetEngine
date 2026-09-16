(Engine/Network/INetworkTransport.h)
## Concept

[[INetworkTransport]] est l'interface bas niveau d'un backend reseau.

Elle definit les operations dont [[NetworkService]] a besoin.

```cpp
class INetworkTransport
{
public:
	virtual bool StartServer(uint16_t port, uint32_t maxPeers) = 0;
	virtual bool Connect(const EndPoint& endpoint) = 0;
	virtual void Send(PeerId peer, ChannelId channel, std::span<const std::byte> payload, DeliveryType mode) = 0;
	virtual std::vector<NetworkEvent::Type> PollEvents() = 0;
};
```

L'engine peut remplacer le backend sans changer le code gameplay.

