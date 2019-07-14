#pragma once
#include "System.h"
#include "../entities/components/Components.h"
#include "../entities/events/Events.h"

class EntityManager;

/*
	Example system that acts on ComponentA and EventA (UnTargeted).
*/
class SystemA : public System<ComponentA, EventA>
{
	friend class EntityManager;
public:
	explicit SystemA() {};
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

	void updateEntity(const double& dt, UnitGroup& units) override
	{
		// The unit group will contain the unit types that the System template was instantiated with.
		ComponentA* a = units.get<ComponentA>().first;
		size_t aSize = units.get<ComponentA>().second;

		EventA& b = units.get<EventA>().first[0];

		for(size_t i = 0; i < aSize; i++)
			if (b.b)
			{
				a[i].x += (float)dt;
				a[i].y += (float)dt;
				a[i].z += (float)dt;
			}
	}
};

/*
	Example system that acts on ComponentA and EventB (Targeted).
*/
class SystemB : public System<ComponentA, EventB>
{
	friend class EntityManager;
public:
	explicit SystemB() {};
private:
	void update(const double& dt) override
	{
		updateEntities(dt);
	}

	void updateEntity(const double& dt, UnitGroup& units) override
	{
		ComponentA& a = units.get<ComponentA>().first[0];
		EventB& b = units.get<EventB>().first[0];

		if (b.b)
		{
			a.x += (float)dt;
			a.y += (float)dt;
			a.z += (float)dt;
		}
	}
};