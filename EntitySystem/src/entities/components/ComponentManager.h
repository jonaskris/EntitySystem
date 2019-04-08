#pragma once
#include <vector>
#include <typeinfo>
#include "Component.h"
#include "ComponentManagerIterator.h"
#include "../Entity.h"

// ComponentManager is a sorted collection of a specified type of component.
class ComponentManagerBase
{
public:
	// Inserts a component into the collection.
	virtual void insertComponent(const Entity& entity, ComponentBase* component) = 0;

	virtual ComponentBase* at(const size_t& index) = 0;

	// Gets the entity associated with component at index
	virtual Entity entityAtIndex(const size_t& index) const = 0;

	// Inserts a component into the collection after checking that it stores the component type.
	template <typename ComponentType>
	bool safeInsertComponent(const Entity& entity, ComponentType* component) { 
		if (storesComponentType<ComponentType>())
		{
			insertComponent(entity, component);
			return true;
		} else {
			return false;
		}
	};

	// Returns true if it found any components to erase. 
	virtual bool eraseComponents(const Entity& entity) = 0; //! Not finished!

	// Returns an iterator to the beginning of the ComponentManager
	ComponentManagerIterator begin() { return ComponentManagerIterator(0, this); };

	// Returns true if it stores the specified component type
	template <typename ComponentType>
	bool storesComponentType() { 
		return storesTypeHashed(typeid(ComponentType).hash_code()); 
	};

	virtual bool storesTypeHashed(const size_t& hash) = 0;

	virtual size_t size() = 0;
};

// Wraps component to assign it to entity
template <typename ComponentType>
struct ComponentWrapper
{
	static_assert(std::is_base_of<ComponentBase, ComponentType>::value, "ComponentType of ComponentWrapper must be derived from ComponentBase!");

	Entity entity;
	ComponentType component;
	ComponentWrapper(const Entity& entity, const ComponentType& component) : entity(entity), component(component) {};
};

template <typename ComponentType>
class ComponentManager : public ComponentManagerBase
{
	static_assert(std::is_base_of<ComponentBase, ComponentType>::value, "ComponentType of ComponentManager must be derived from ComponentBase!");
	friend class ComponentManagerIterator;

	std::vector<ComponentWrapper<ComponentType>> componentVector;
public:

	ComponentManager(const size_t& InitialCapacity)
	{
		componentVector.reserve(InitialCapacity);
	};

	~ComponentManager()
	{
		
	};

	// Inserts sorted, beginning from the back of the array
	void insertComponent(const Entity& entity, ComponentBase* component) override
	{
		//static_assert(std::is_same<ComponentBase, ComponentType>::value, "ComponentType of ComponentManager must be derived from ComponentBase!");

		ComponentType* casted = static_cast<ComponentType*>(component);

		for (auto it = componentVector.rbegin(); it != componentVector.rend(); it++)
		{
			if (entity > (*it).entity)
			{
				componentVector.insert(it.base(), ComponentWrapper{ entity, *casted });
				return;
			}
		}
		componentVector.push_back(ComponentWrapper{ entity, *casted });
	}

	ComponentBase* at(const size_t& index)
	{
		return reinterpret_cast<ComponentBase*>(&(componentVector.at(index)));
	}

	bool eraseComponents(const Entity& entity) override  //! Not finished!
	{
		for (auto it = componentVector.begin(); it != componentVector.end(); it++)
		{
			if (it->entity == entity)
			{
				componentVector.erase(it);
				return true;
			}
		}
		return false;
	}

	Entity entityAtIndex(const size_t& index) const
	{
		return componentVector.at(index).entity;
	}

	bool storesTypeHashed(const size_t& hash) override { return typeid(ComponentType).hash_code() == hash; };
	size_t size() override { return componentVector.size(); };
};