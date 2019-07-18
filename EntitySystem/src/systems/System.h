#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include "../entitymanager/EntityManager.h"
#include "../units/UnitGroup.h"
#include "../entitymanager/EachCallable.h"

/*
	Base of every system.
*/
class SystemBase
{
	friend class EntityManager;
private:
	void setEntityManager(EntityManager* entityManager) { this->entityManager = entityManager; };
protected:
	EntityManager* entityManager = nullptr;

	virtual ~SystemBase() { };

	/*
		Called on every EntityManager.update once every update before updating individual entities.
	*/
	virtual void update(const double& dt) = 0;

	/*
		Called on every EntityManager.update for each set of entities with types UnitTypes.
	*/
	virtual void updateEntity(UnitGroup& units) = 0;
public:
};

/*
	Base class of systems.
	Defines operations on a set of units that has the same entity (Except event), which execute on every update of EntityManager.
*/
template <typename UnitsClass, typename OptionalUnitsClass = OptionalUnits<>>
class System : public SystemBase, EachCallable<UnitsClass, OptionalUnitsClass>
{
	friend class EntityManager;
private:
	void eachCall(UnitGroup& unitGroup) override { updateEntity(unitGroup); }
	void update(const double& dt) override
	{ 
		this->dt = dt;
		update();
		this->entityManager->each(this);
	}

	virtual void update() = 0;
protected:
	double dt = 0;
};