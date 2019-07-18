#pragma once
#include "System.h"
#include "../components/Components.h"
#include "../events/Events.h"
#include "../entitymanager/EachCallable.h"

class EntityManager;

/*
	Example system that acts on ComponentA and EventA (UnTargeted).
*/
class SystemA : public System< Units<ComponentA, EventA> >
{
	friend class EntityManager;
public:
	explicit SystemA() {};
private:
	/*
		Update is called on every update of EntityManager, as long as it is registered in the EntityManager.
	*/
	void update() override
	{

	}

	void updateEntity(UnitGroup& units) override
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
class SystemB : public System< Units<ComponentA, EventB> >
{
	friend class EntityManager;
public:
	explicit SystemB() {};
private:
	void update() override
	{
		
	}

	void updateEntity(UnitGroup& units) override
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

class SystemC : public System< Units<ComponentA, EventB>, OptionalUnits<false, true> >
	//public System< ComponentA, EventB::optional>
{
	friend class EntityManager;
public:
	explicit SystemC() {};
private:
	void update() override
	{

	}

	void updateEntity(UnitGroup& units) override
	{
		ComponentA& a = units.get<ComponentA>().first[0];
		ComponentB* b = units.get<ComponentB>().first;
		size_t bs = units.get<ComponentB>().second;

		if (bs == 0 || b->b)
		{
			a.x += (float)dt;
			a.y += (float)dt;
			a.z += (float)dt;
		}
	}
};