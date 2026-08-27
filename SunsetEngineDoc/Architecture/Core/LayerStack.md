Le [[LayerStack]] est un containeur de [[Layer]].
Qui permet d'itérer facilement à travers lui.

```cpp
class LayerStack final
{
public:
	std::vector<std::unique_ptr<Layer>> m_Layers;
}
```
