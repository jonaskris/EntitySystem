#pragma once
#include <map>
#include "Component.h"

class EntityManager;

/*
	Used to pass pointers to groups of components with unique types.
*/
class ComponentGroup
{
	friend class EntityManager;
private:
	// Maps component identifiers to respective components.
	std::map<size_t, ComponentBase*> components;

public:
	template <typename ComponentType>
	ComponentType* get()
	{
		static_assert(std::is_base_of<ComponentBase, ComponentType>::value, "ComponentType must be derived from ComponentBase!");

		std::map<size_t, ComponentBase*>::iterator it = components.find(ComponentType::getIdentifier());

		if (it == components.end())
			return nullptr;

		return static_cast<ComponentType*>(it->second);
	}

};