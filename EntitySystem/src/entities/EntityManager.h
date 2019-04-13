#pragma once
#include <functional>
#include <vector>
#include <algorithm>
#include "components/ComponentManager.h"
#include "components/ComponentManagerIterator.h"
#include "Entity.h"
#include "../systems/System.h"

#define EM_COMPONENTMANAGER_DEFAULT_INITIAL_CAPACITY 1024

struct ComponentBase;

/*
	Stores entities, systems and component managers,
	and defines their relationship.
*/
class EntityManager
{
private:
	/*
		Systems
	*/
	std::vector<System*> systems;

	/*
		Entities
	*/
	std::vector<ComponentManagerBase*> componentManagers;
	unsigned int entityCount = 0;
	unsigned int entityCounter = 1; // For assigning entity id, 0 is null value

private:
	/*
		Creates a new componentmanager of specified type.
		Expects that a check is already done as to whether an existing componentManager stores the given type,
		if not, unexpected behaviour may occur.
	*/
	template <typename ComponentType>
	void newComponentManager()
	{
		componentManagers.push_back(new ComponentManager<ComponentType>(EM_COMPONENTMANAGER_DEFAULT_INITIAL_CAPACITY));
	}

	/*
		Unpacks parameter pack of components when creating an entity, base case.
	*/
	template <typename C>
	void unpackAndStoreComponentsInManagers(Entity e, C c)
	{
		newComponent(e, c);
	}

	/*
		Unpacks parameter pack of components when creating an entity, recursive case.
	*/
	template <typename C, typename... Rest>
	void unpackAndStoreComponentsInManagers(Entity e, C c, Rest... rest)
	{
		newComponent(e, c);
		unpackAndStoreComponentsInManagers(e, rest...);
	}

	/*
		Unpacks parameter pack of ComponentTypes to get a vector of component manager iterators of their respective component managers, base case.
	*/
	template <typename C>
	std::vector<ComponentManagerIterator>& unpackAndGetComponentManagersIteratorsHelper(std::vector<ComponentManagerIterator>& vec)
	{
		for (size_t i = 0; i < this->componentManagers.size(); i++)
		{
			if (this->componentManagers.at(i)->storesComponentType<C>())
			{
				vec.push_back(this->componentManagers.at(i)->begin());
				return vec;
			}
		}
		return vec;
	}

	/*
		Unpacks parameter pack of ComponentTypes to get a vector of component manager iterators of their respective component managers, recursive case.
	*/
	template <typename C, typename F, typename... Rest>
	std::vector<ComponentManagerIterator>& unpackAndGetComponentManagersIteratorsHelper(std::vector<ComponentManagerIterator>& vec)
	{
		for (size_t i = 0; i < this->componentManagers.size(); i++)
		{
			if (this->componentManagers.at(i)->storesComponentType<F>())
			{
				vec.push_back(this->componentManagers.at(i)->begin());
				return unpackAndGetComponentManagersIteratorsHelper<C, Rest...>(vec);
			}
		}
		return unpackAndGetComponentManagersIteratorsHelper<C, Rest...>(vec);
	}

	/*
		Unpacks parameter pack of ComponentTypes to get a vector of component manager iterators of their respective component managers.
	*/
	template <typename... Rest>
	std::vector<ComponentManagerIterator> unpackAndGetComponentManagersIterators()
	{
		std::vector<ComponentManagerIterator> vec;
		return unpackAndGetComponentManagersIteratorsHelper<Rest...>(vec);
	}

public:
	EntityManager() {};
	~EntityManager() 
	{
		for (size_t i = 0; i < componentManagers.size(); i++)
		{
			delete componentManagers[i];
		}

		for (size_t i = 0; i < systems.size(); i++)
		{
			delete systems[i];
		}
	};

	/*
		Defines an entity.
		Returns the entity.
	*/
	Entity newEntity()
	{
		Entity newEntity{ entityCounter };
		entityCount++;
		entityCounter++;

		return newEntity;
	}

	/*
		Defines an entity and stores components assigned to the new entity.
		Returns the entity.
	*/
	template <typename... Components>
	Entity newEntity(Components... components)
	{
		Entity newEntity{ entityCounter };
		entityCount++;
		entityCounter++;

		unpackAndStoreComponentsInManagers(newEntity, std::forward<Components>(components)...);
		return newEntity;
	}


	/*
		Stores a component in componentmanagers and assigns it to the specified entity.
	*/
	template <typename ComponentType>
	void newComponent(Entity e, ComponentType component)
	{
		static_assert(std::is_base_of<ComponentBase, ComponentType>::value, "ComponentType of newComponent must be derived from ComponentBase!");

		// Try storing component in an existing manager.
		for (size_t i = 0; i < componentManagers.size(); i++)
			if (componentManagers.at(i)->storesComponentType<ComponentType>())
			{
				componentManagers.at(i)->insertComponent(e, &component);
				return;
			}

		// If unsuccessful in storing component above, there were no component manager that stores given ComponentType.
		// Create a new componentManager that stores the component type, and insert the component into the new manager. 
		newComponentManager<ComponentType>();
		componentManagers.back()->insertComponent(e, &component);
	}

	/*
		Registers a system that will be used to update components on update call.
		ConstructorArgs are optional.
	*/
	template <typename SystemType, typename... ConstructorArgs>
	void registerSystem(ConstructorArgs... constructorArgs)
	{
		static_assert(std::is_base_of<System, SystemType>::value, "Can only register systems that are derived from System class!");

		systems.push_back(new SystemType(constructorArgs...));
		systems.back()->setEntityManager(this);
	}

	/* 
		Executes a lambda on every entity that has specified set of component types.
	*/
	template <typename... ComponentTypes>
	void each(std::function<void(std::vector<ComponentBase*>&)> lambda)
	{
		static_assert(sizeof...(ComponentTypes) > 0, "Cannot iterate over no components! Specify set of component types to iterate over in template argument.");

		// Get each relevant componentmanager and create an iterator for it, if there's no componentmanager that stores one of the types; return. 
		std::vector<ComponentManagerIterator> componentManagerIterators = unpackAndGetComponentManagersIterators<ComponentTypes...>();

		// Check if iterator/manager was found for each of ComponentTypes.
		if (sizeof...(ComponentTypes) > componentManagerIterators.size())
			return;

		// Since unpacking iterators results in reversed order, it must be reversed again to get original order.
		std::reverse(componentManagerIterators.begin(), componentManagerIterators.end());

		// While first iterator is not at end.
		do
		{
			for (size_t i = 1; i < componentManagerIterators.size(); i++)
			{

				// While entity is smaller than first iterators entity.
				while (componentManagerIterators.at(i).getCurrentEntity() < componentManagerIterators.at(0).getCurrentEntity())
					componentManagerIterators.at(i).increment();

				/* 
					If entity is bigger than first iterators entity, the first iterators entity doesent have specified set of components. 
					Continue by ignoring this entity and continuing with the next entity of the first iterator. 
				*/
				if (!(componentManagerIterators.at(i).getCurrentEntity() == componentManagerIterators.at(0).getCurrentEntity()))
					goto continueOuter;
								
			}

			/* 
				If reached this point, lambda can be executed with the components that iterators point to.
				Get component of each iterator and execute lambda with vector of these components 
			*/
			{
				std::vector<ComponentBase*> components;
				for (size_t i = 0; i < componentManagerIterators.size(); i++)
					components.push_back(componentManagerIterators.at(i).getCurrent());
				lambda(components);

			}

			continueOuter:;
		} while (componentManagerIterators.at(0).increment());
	}

	/*
		Executes every registered systems lambda on entities that consists of their respective ComponentTypes.
	*/
	void update(const double& dt)
	{
		for (size_t i = 0; i < systems.size(); i++)
		{
			systems.at(i)->update(dt);
		}
	}


	size_t sizeEntities() { return entityCount; };
	size_t sizeComponentManagers() { return componentManagers.size(); };
	size_t sizeSystems() { return systems.size(); };

	/*
		Retrieves a copy of a component, used for testing purposes.
	*/
	template <typename ComponentType>
	ComponentType getComponentCopyFromEntity(Entity e)
	{
		for (size_t i = 0; i < componentManagers.size(); i++)
		{
			if (componentManagers.at(i)->storesComponentType<ComponentType>())
			{
				return *static_cast<ComponentType*>(componentManagers.at(i)->getComponentFromEntity(e));
			}
		}
		return *static_cast<ComponentType*>(nullptr);
	}
};