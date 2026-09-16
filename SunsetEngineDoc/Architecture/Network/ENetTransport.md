(Engine/Network/ENet/ENetTransport.h)
## Concept

[[ENetTransport]] est l'implementation de [[INetworkTransport]] avec ENet.

Il gere:

- la creation du host serveur
- la connexion client
- l'envoi reliable ou unreliable
- le polling des events ENet
- le shutdown du host

```cpp
std::unique_ptr<INetworkTransport> transport = std::make_unique<ENetTransport>();
transport->StartServer(7777, 8);
```

[[NetworkService]] ne depend que de l'interface [[INetworkTransport]].

