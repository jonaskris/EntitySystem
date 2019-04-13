#include "ComponentManagerIterator.h"
#include "ComponentManager.h"
#include "../Entity.h"

struct ComponentBase;

bool ComponentManagerIterator::sameEntity(const ComponentManagerIterator& other) const
{
	return componentManager->entityAtIndex(index) == other.componentManager->entityAtIndex(other.index);
};

Entity ComponentManagerIterator::getCurrentEntity() { return componentManager->entityAtIndex(index); };

ComponentBase* ComponentManagerIterator::getCurrent() const 
{
	return componentManager->at(index);
};

bool ComponentManagerIterator::increment()
{
	if (index == (componentManager->size() - 1))
		return false;
	index++;
	return true;
};

bool ComponentManagerIterator::decrement()
{
	if (index == 0)
		return false;
	index--;
	return true;
};

bool ComponentManagerIterator::operator < (const ComponentManagerIterator& other) const
{
	return (componentManager->size() < other.componentManager->size());
}