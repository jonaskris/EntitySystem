#include <iostream>
#include "entities/components/Component.h"
#include "entities/components/ComponentManager.h"
#include "entities/Entity.h"

int main()
{
	ComponentA a{0.0f, 1.0f, 2.0f};
	ComponentB b{false};

	ComponentManagerBase* componentManagerA = new ComponentManager<ComponentA>(32);
	ComponentManagerBase* componentManagerB = new ComponentManager<ComponentB>(32);

	// Inserting into componentManagerA 
	componentManagerA->insertComponent(Entity{1}, &a);
	componentManagerA->insertComponent(Entity{2}, &b);
	componentManagerA->safeInsertComponent(Entity{3}, &a);
	componentManagerA->safeInsertComponent(Entity{4}, &b); // Fails as type check comes out as false

	// Inserting into componentManagerB
	componentManagerB->insertComponent(Entity{1}, &b);
	componentManagerB->insertComponent(Entity{2}, &a);
	componentManagerB->insertComponent(Entity{3}, &a);
	componentManagerB->safeInsertComponent(Entity{4}, &a); // Fails as type check comes out as false
	componentManagerB->safeInsertComponent(Entity{5}, &b);


	std::cout << "ManagerA storesComponentType A, should be True, is: " << (componentManagerA->storesComponentType<ComponentA>() ? ("True") : ("False")) << std::endl;
	std::cout << "ManagerA storesComponentType B, should be False, is: " << (componentManagerA->storesComponentType<ComponentB>() ? ("True") : ("False")) << std::endl;
	std::cout << "ManagerB storesComponentType B, should be True, is: " << (componentManagerB->storesComponentType<ComponentB>() ? ("True") : ("False")) << std::endl;
	std::cout << "ManagerB storesComponentType A, should be False, is: " << (componentManagerB->storesComponentType<ComponentA>() ? ("True") : ("False")) << std::endl;

	std::cout << "Size of ManagerA, should be 3, is: " << componentManagerA->size() << std::endl;
	std::cout << "Size of ManagerB, should be 4, is: " << componentManagerB->size() << std::endl;

	system("PAUSE");
}