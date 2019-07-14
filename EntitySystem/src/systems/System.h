#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include "../entities/events/Event.h"
#include "../entities/components/Component.h"
#include "../entities/units/UnitGroup.h"


class EntityManager;

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
	virtual void updateEntity(const double& dt, UnitGroup& units) = 0;
public:
};

/*
	Base class of systems.
	Defines operations on a set of units that has the same entity (Except event), which execute on every update of EntityManager.
*/
template <typename... UnitTypes>
class System : public SystemBase
{
	friend class EntityManager;

	/*
		Identifiers of every type of unit the system should act on.
	*/
	static std::vector<size_t> unitIdentifiers;

	/*
		Unpacks parameter pack of unit types, base case.
	*/
	template <typename UnitType>
	static void unpackUnitTypesHelper(std::vector<size_t>& unitIdentifiersUnpacking)
	{
		static_assert(std::is_base_of<Unit, UnitType>::value && std::is_base_of<UnitTypeIdentifier<UnitType>, UnitType>::value, "UnitType must be derived from Unit and UnitTypeIdentifier!");

		unitIdentifiersUnpacking.push_back(UnitType::getIdentifier());
	}

	/*
		Unpacks parameter pack of unit types, recursive case.
	*/
	template <typename F, typename UnitType, typename... Rest>
	static void unpackUnitTypesHelper(std::vector<size_t>& unitIdentifiersUnpacking)
	{
		static_assert(std::is_base_of<Unit, UnitType>::value && std::is_base_of<UnitTypeIdentifier<UnitType>, UnitType>::value, "UnitType must be derived from Unit and UnitTypeIdentifier!");

		unitIdentifiersUnpacking.push_back(UnitType::getIdentifier());
		unpackUnitTypesHelper<F, Rest...>(unitIdentifiersUnpacking);
	}

	/*
		Unpacks parameter pack of unit types.
	*/
	static std::vector<size_t> unpackUnitTypes()
	{
		std::vector<size_t> unitIdentifiersUnpacking;
		unpackUnitTypesHelper<UnitTypes...>(unitIdentifiersUnpacking);

		std::reverse(unitIdentifiersUnpacking.begin(), unitIdentifiersUnpacking.end());

		return unitIdentifiersUnpacking;
	}

protected:
	System() { };

	/*
		To be called by System.Update by each specific type of System whenever needed.
	*/
	void updateEntities(const double& dt)
	{
		this->entityManager->each(dt, this, unitIdentifiers);
	}
};
template <typename... UnitTypes>
std::vector<size_t> System<UnitTypes...>::unitIdentifiers = System<UnitTypes...>::unpackUnitTypes();
