#include <iostream>
#include "System.h"
#include "../entities/EntityManager.h"

void System_Example::update(const double& dt)
{
	static std::function updateLambda =
		[&](std::vector<ComponentBase*>& components)
	{
		ComponentA* a = static_cast<ComponentA*>(components.at(0));
		ComponentB* b = static_cast<ComponentB*>(components.at(1));

		if (b->b)
		{
			a->x += (float)dt;
			a->y += (float)dt;
			a->z += (float)dt;
		}
	}; 

	if (entityManager)
		entityManager->each<ComponentA, ComponentB>(updateLambda);
}