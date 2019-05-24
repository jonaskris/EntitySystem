#pragma once
#include "System.h"

class EntityManager;

/*
	Example system that acts on ComponentA and ComponentB.
*/
class System_Example : public System<System_Example>
{
	friend class EntityManager;
public:
	explicit System_Example() {};
private:
	/*
		Update is called on every update of entitymanager, as long as it is registered in the entitymanager.
	*/
	void update(const double& dt) override
	{
		std::function updateLambda =
			[&](std::vector<ComponentBase*> & components)
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

		/*
			Components to act on and the recieved order in lamda vector is defined by template arguments of each.
			To improve performance, one should consider not calling each on every update call, but at set intervals based on dt or 
			other variables of the systems internal state.
		*/
		entityManager->each<ComponentA, ComponentB>(updateLambda);
	}
};

/*
	To test unique identifiers.
*/
class System_Test : public System<System_Test>
{
	friend class EntityManager;
public:
	explicit System_Test() {};
private:
	void update(const double& dt) override
	{

	}
};