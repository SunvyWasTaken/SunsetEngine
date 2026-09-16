(Engine/Network/NetworkService.h)
## Concept

[[NetworkService]] est l'interface haut niveau du reseau.

Il possede un [[INetworkTransport]] et cache l'implementation concrete comme [[ENetTransport]].

```cpp
NetworkService::Init();
NetworkService::Get().Host(7777, 8);
NetworkService::Get().Update(dt);
```

Il peut etre serveur ou client.

```cpp
service.Host(port, maxPlayers);
service.Join(endpoint);
```

## Messages

Les messages reseau doivent etre `trivially_copyable`.

Un message peut declarer son channel directement:

```cpp
struct PlayerInput
{
	static constexpr uint8_t ChannelId = 1;
	float x;
	float y;
};
```

Sinon il faut l'enregistrer.

```cpp
service.RegisterMessage<PlayerInput>(1);
```

## Dispatch

[[NetworkService]] transforme les packets bruts en callbacks types.

```cpp
service.RegisterHandler<PlayerInput>(
	[](PeerId peer, const PlayerInput& input)
	{
		// ...
	}
);
```

