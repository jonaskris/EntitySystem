#pragma once
#include <vector>
#include <typeinfo>
#include "Component.h"
#include "ComponentManagerIterator.h"
#include "../Entity.h"

/* 
	ComponentManager is a sorted collection of a specified type of component.
*/
class ComponentManagerBase
{
	/* 
		A virtual helper method to insert component into derived classes vector.
	*/
	virtual void insertComponentVirtual(const Entity& e, ComponentBase* c) = 0;

	/*
		A virtual helper method that checks if the derived class stores a components assigned id.
		Components are assigned id from ComponentIdentifier.
	*/
	virtual bool storesTypeId(const size_t& id) const = 0;
public:
	/*
		Insures proper destruction of derived class.
	*/
	virtual ~ComponentManagerBase() { };

	/*
		Gets pointer to component at specified index.
	*/
	virtual ComponentBase* at(const size_t& index) = 0;

	/*
		Gets pointer to component belonging to specified entity.
	*/
	virtual ComponentBase* getComponentFromEntity(Entity e) = 0;

	/* 
		Inserts a component into the collection after checking that it stores the component type. 
	*/
	template <typename ComponentType>
	bool insertComponent(const Entity& entity, ComponentType* component) {
		static_assert(std::is_base_of<ComponentBase, ComponentType>::value, "ComponentType of ComponentManager.insertComponent must be derived from ComponentBase.");

		if (storesComponentType<ComponentType>())
		{
			insertComponentVirtual(entity, component);
			return true;
		}
		else {
			return false;
		}
	};

	/* 
		Erases components belonging to specified entity.
		Returns true if it found any components to erase. 
	*/
	virtual bool eraseComponents(const Entity& entity) = 0; //! Not finished!

	/* 
		Returns a ComponentManagerIterator to the beginning of the ComponentManager.
	*/
	ComponentManagerIterator begin() { return ComponentManagerIterator(0, this); };

	template <typename ComponentType>
	bool storesComponentType() {
		return storesTypeId(ComponentIdentifier<ComponentType>::getIdentifier());
	};

	virtual Entity entityAtIndex(const size_t& index) const = 0;

	virtual size_t size() const = 0;
};

/* 
	Wraps component to assign it an entity.
*/
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

	/* 
		Inserts sorted, beginning from the back of the array.
	*/
	void insertComponentVirtual(const Entity& e, ComponentBase* c) override
	{
		ComponentType* casted = static_cast<ComponentType*>(c);

		for (auto it = componentVector.rbegin(); it != componentVector.rend(); it++)
		{
			if (e > (*it).entity)
			{
				componentVector.insert(it.base(), ComponentWrapper{ e, *casted });
				return;
			}
		}
		componentVector.push_back(ComponentWrapper{ e, *casted });
	}

	/*
		Checks that the component manager stores component with specified identifier.
		Components are assigned id from ComponentIdentifier.
	*/
	bool storesTypeId(const size_t& id) const override { return ComponentIdentifier<ComponentType>::getIdentifier() == id; };
public:

	ComponentManager(const size_t& InitialCapacity)
	{
		componentVector.reserve(InitialCapacity);
	};

	~ComponentManager() { };


	/*
		Gets pointer to component at specified index.
	*/
	ComponentBase* at(const size_t& index) override
	{
		return &(componentVector.at(index).component);
	}

	/*
		Gets pointer to component belonging to specified entity.
	*/
	ComponentBase* getComponentFromEntity(Entity e) override
	{
		for (size_t i = 0; i < componentVector.size(); i++) 
		{
			if (componentVector.at(i).entity == e)
			{
				return static_cast<ComponentBase*>(&componentVector.at(i).component);
			}
		}
		return nullptr;
	}

	/*
		Erases components belonging to specified entity.
		Returns true if it found any components to erase.
	*/
	bool eraseComponents(const Entity& e) override
	{
		for (auto it = componentVector.begin(); it != componentVector.end(); it++)
		{
			if (it->entity == e)
			{
				componentVector.erase(it);
				return true;
			}
		}
		return false;
	}

	Entity entityAtIndex(const size_t& index) const override
	{
		return componentVector.at(index).entity;
	}

	size_t size() const override { return componentVector.size(); };
};