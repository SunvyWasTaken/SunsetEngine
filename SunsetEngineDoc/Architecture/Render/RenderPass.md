
```txt
Un RenderPass décrit une étape logique du rendu, mais c'est son exécution qui orchestre concrètement les commandes de rendu nécessaires pour produire ses outputs.
```

Le render pass c'est une étapes du Pipeline de rendu.
Il reçoit des données qu'il va transformer en suivant la méthode qu'on lui à donnée.
Le résultat est ensuite stocker dans un [[Framebuffer]]. A savoir que le RenderPass n'as pas accès à sont résultat (à cause d'optimisation matériel).
Par contr un [[RenderPass]] peut possèder des Subpass si la même pass doit être effectuer plusieurs fois ou si il y a plusieurs étapes à faire en une (de ce que j'ai compris).

```cpp
class RenderPass
{
public:
	virtual ~RenderPass() = default;
	virtual void operator()(RenderContext& context)
	{
		// ici je dois mettre quoi concrètement?...
	}
	// je sais pas si il doit y avoir autre chose en plus...
}
```

<u>Exemple :</u>

```cpp
class BaseColorPass final : public RenderPass
{
	~BaseColorPass() override = default;
	
	void operator()(RenderContext& context) override
	{
		
	}
}
```
