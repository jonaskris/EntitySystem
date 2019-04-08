#include "ComponentManagerIterator.h"
#include "ComponentManager.h"
#include "Component.h"

bool ComponentManagerIterator::sameEntity(const ComponentManagerIterator& other) const
{
	return componentManager->entityAtIndex(index) == other.componentManager->entityAtIndex(other.index);
};

ComponentBase* ComponentManagerIterator::getCurrent() {
	return static_cast<ComponentBase*>(componentManager->at(index));
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

bool ComponentManagerIterator::compare(const ComponentManagerIterator& other) const
{
	return componentManager->entityAtIndex(index) > other.componentManager->entityAtIndex(other.index);
}
