#pragma once
#include <vector>
#include <functional>
#include "../entities/components/Component.h"

class EntityManager;

class System
{
	friend class EntityManager;
protected:
	EntityManager* entityManager = nullptr;
	System() { };
	virtual ~System() { };

	void setEntityManager(EntityManager* entityManager) { this->entityManager = entityManager; };
public:
	virtual void update(const double& dt) = 0;
};

class System_Example : public System
{
public:
	System_Example() { };
	void update(const double& dt) override;
};