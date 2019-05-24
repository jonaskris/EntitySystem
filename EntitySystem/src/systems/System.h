#pragma once
#include <vector>
#include <functional>

class EntityManager;

/*
	Wraps a global variable that ensures SystemIdentifier has an unique identifier for each SystemType.
*/
class SystemIdentifierCounter
{
	template <typename SystemType>
	friend class SystemIdentifier;

	static size_t counter;
};

/*
	Base of every system.
*/
class SystemBase
{
	friend class EntityManager;
private:
	virtual void setEntityManager(EntityManager* entityManager) = 0;
protected:
	virtual ~SystemBase() { };
	virtual void update(const double& dt) = 0;
public:
	virtual size_t getIdentifier() const = 0;
};

/*
	Identifies a system with a unique identifier.
*/
template <typename SystemType>
class SystemIdentifier 
{
	static size_t systemTypeIdentifier;
public:
	static size_t getIdentifierStatic() { return systemTypeIdentifier; }

};
template <typename SystemType>
size_t SystemIdentifier<SystemType>::systemTypeIdentifier = SystemIdentifierCounter::counter++;

/*
	Base class of systems.
	Defines operations on a set of components that has the same entity, which execute on every update of EntityManager.
	Can only be instantiated through EntityManager's registerSystem.
*/
template <typename SystemType>
class System : public SystemBase, SystemIdentifier<SystemType>
{
	friend class EntityManager;
protected:
	virtual ~System() { };
	EntityManager* entityManager = nullptr;
	explicit System() { };

	/*
		Called on every EntityManager.update for each set of entities that have the components specified in overridden update method.
	*/
	virtual void update(const double& dt) override = 0;

	void setEntityManager(EntityManager* entityManager) { this->entityManager = entityManager; };
public:
	virtual size_t getIdentifier() const override { return SystemIdentifier<SystemType>::getIdentifierStatic(); }
};
