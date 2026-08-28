
Ok [[DrawCmd]] est juste une struct qui va contenir de la donnée.

```cpp
struct DrawCmd
{
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	glm::mat4 model = glm::mat4(1.f);
};
```
