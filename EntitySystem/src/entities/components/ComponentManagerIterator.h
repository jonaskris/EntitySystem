#pragma once
//#include "ComponentManager.h"
#include "Component.h"

class ComponentManagerBase;

class ComponentManagerIterator
{
private:
	friend class ComponentManagerBase;
	size_t index = 0;
	ComponentManagerBase* componentManager;
	ComponentManagerIterator(const size_t& index, ComponentManagerBase* componentManager) : componentManager(componentManager), index(index) { };
public:

	// Increments index, returns false if out of bounds.
	bool increment();

	// Decrements index, returns false if out of bounds.
	bool decrement();

	// Checks if Entity at index of two ComponentManagerIterators are the same.
	bool sameEntity(const ComponentManagerIterator& other) const;

	// Returns true if Entity at index of this ComponentManagerIterator is bigger than other.
	bool compare(const ComponentManagerIterator& other) const;

	// Gets the component at current index.
	ComponentBase* getCurrent();
};

//template <typename ComponentType>
//class ComponentManagerIterator
//{
//
//};