#pragma once
#include "../Entity.h"

class ComponentManagerBase;
struct ComponentBase;

/*
	A generic iterator for use with ComponentManager.
*/
class ComponentManagerIterator
{
private:
	friend class ComponentManagerBase;

	size_t index = 0;
	ComponentManagerBase* componentManager;
	ComponentManagerIterator(const size_t& index, ComponentManagerBase* componentManager) : componentManager(componentManager), index(index) { };
public:

	/* 
		Returns false if incrementing leads to out of bounds. 
	*/
	bool increment();

	/* 
		Returns false if decrementing leads to out of bounds. 
	*/
	bool decrement();

	/* 
		Returns if Entity at index of two ComponentManagerIterators are the same. 
	*/
	bool sameEntity(const ComponentManagerIterator& other) const;

	Entity getCurrentEntity();

	/* 
		Gets the component at current index.
	*/
	ComponentBase* getCurrent() const;

	/* 
		For sorting iterators by size of the componentmanager.
	*/
	bool operator < (const ComponentManagerIterator& other) const;
};