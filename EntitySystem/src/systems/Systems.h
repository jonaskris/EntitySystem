#pragma once
#include "System.h"

class EntityManager;

/*
	Example system that acts on ComponentA and ComponentB.
*/
class System_Example : public System<ComponentA, ComponentB>
{
	friend class EntityManager;
public:
	explicit System_Example() {};
private:
	/*
		Update is called on every update of EntityManager, as long as it is registered in the EntityManager.
	*/
	void update(const double& dt) override
	{
		/*
			To improve performance, one should consider not calling updateEntities on every update call, but at set intervals based on dt or 
			other variables of the systems internal state. (But completely fine to call every update)
		*/
		updateEntities(dt);
	}

	void updateEntity(const double& dt, ComponentGroup& components) override
	{
		ComponentA* a = components.get<ComponentA>();
		ComponentB* b = components.get<ComponentB>();

		if (b->b)
		{
			a->x += (float)dt;
			a->y += (float)dt;
			a->z += (float)dt;
		}
	}
};

/*
	To test unique identifiers.
*/
class System_Test : public System<>
{
	friend class EntityManager;
public:
	explicit System_Test() {};
private:
	void update(const double& dt) override
	{

	}

	void updateEntity(const double& dt, ComponentGroup& components) override
	{

	}
};