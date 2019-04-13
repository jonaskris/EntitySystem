#include <iostream>
#include <chrono>
#include <functional>
#include "entities/components/Component.h"
#include "entities/components/ComponentManager.h"
#include "entities/components/ComponentManagerIterator.h"
#include "entities/EntityManager.h"
#include "entities/Entity.h"
#include "systems/System.h"

#define RUN_TESTS true
#define RUN_MEMORY_LEAK_TEST false

int main()
{

#if RUN_TESTS
{
	{
		ComponentA a{ 0.0f, 1.0f, 2.0f };
		ComponentB b{ false };

		ComponentManagerBase* componentManagerA = new ComponentManager<ComponentA>(1024);
		ComponentManagerBase* componentManagerB = new ComponentManager<ComponentB>(1024);

	// Testing ComponentManager.
		std::cout << "Testing ComponentManager..." << std::endl;
		// Inserting into componentManagerA .
		componentManagerA->insertComponent(Entity{ 1 }, &a);
		componentManagerA->insertComponent(Entity{ 2 }, &b); // Fails as type check comes out as false
		componentManagerA->insertComponent(Entity{ 3 }, &a);
		componentManagerA->insertComponent(Entity{ 4 }, &b); // Fails as type check comes out as false

		// Inserting into componentManagerB.
		componentManagerB->insertComponent(Entity{ 1 }, &b);
		componentManagerB->insertComponent(Entity{ 2 }, &a); // Fails as type check comes out as false
		componentManagerB->insertComponent(Entity{ 3 }, &b);
		componentManagerB->insertComponent(Entity{ 4 }, &a); // Fails as type check comes out as false



		std::cout << "\t ManagerA storesComponentType A, should be True, is: " << (componentManagerA->storesComponentType<ComponentA>() ? ("True") : ("False")) << std::endl;
		std::cout << "\t ManagerA storesComponentType B, should be False, is: " << (componentManagerA->storesComponentType<ComponentB>() ? ("True") : ("False")) << std::endl;
		std::cout << "\t ManagerB storesComponentType B, should be True, is: " << (componentManagerB->storesComponentType<ComponentB>() ? ("True") : ("False")) << std::endl;
		std::cout << "\t ManagerB storesComponentType A, should be False, is: " << (componentManagerB->storesComponentType<ComponentA>() ? ("True") : ("False")) << std::endl << std::endl;
		
		std::cout << "\t Size of manager A, should be 2, is: " << componentManagerA->size() << std::endl;
		std::cout << "\t Size of manager B, should be 2, is: " << componentManagerB->size() << std::endl << std::endl;
		
		delete componentManagerA;
		delete componentManagerB;
	}
	// Testing EntityManager...
	std::cout << "Testing EntityManager..." << std::endl;
	{
		std::cout << "\t Testing newEntity..." << std::endl;

		EntityManager e;
		Entity e1 = e.newEntity(ComponentA{ 1.1f, 1.2f, 1.3f }, ComponentB{ true });
		Entity e2 = e.newEntity(ComponentA{ 2.1f, 2.2f, 2.3f }, ComponentB{ false });
		std::cout << "\t\t Testing EntityManager.newEntity(Components...) and EntityManager.sizeEntities(), size should be 2, is: " << e.sizeEntities() << std::endl;

		Entity e3 = e.newEntity();
		Entity e4 = e.newEntity();
		std::cout << "\t\t Testing EntityManager.newEntity(void) and EntityManager.sizeEntities(), size should be 4, is: " << e.sizeEntities() << std::endl;

		std::cout << "\t Testing newComponent..." << std::endl;
		e.newComponent(e3, ComponentA{ 3.1f, 3.2f, 3.3f });
		e.newComponent(e3, ComponentB{ true });
		e.newComponent(e4, ComponentA{ 4.1f, 4.2f, 4.3f });
		e.newComponent(e4, ComponentB{ false });

		std::cout << "\t\t Testing EntityManager.newComponent(Component) and EntityManager.sizeEntities(), size should be 4, is: " << e.sizeEntities() << std::endl;

		std::cout << "\t Testing each<Components...>(std::function<void(std::vector<ComponentBase*>))" << std::endl;

		// Increments x, y and z in ComponentA if boolean in ComponentB is true.
		e.each<ComponentA, ComponentB>(
			[&](std::vector<ComponentBase*> components) -> void
		{
			ComponentA* a = static_cast<ComponentA*>(components.at(0));
			ComponentB* b = static_cast<ComponentB*>(components.at(1));

			if (b->b)
			{
				a->x++;
				a->y++;
				a->z++;
			}
		});

		std::cout << "\t\t Entity 1 ComponentA should have the values 2.1, 2.2, 2.3, has: ";
		ComponentA e1a = e.getComponentCopyFromEntity<ComponentA>(e1);
		ComponentB e1b = e.getComponentCopyFromEntity<ComponentB>(e1);
		std::cout << e1a.x << ", " << e1a.y << ", " << e1a.z << ", \n\t\t\tComponentB should have value 1, has: " << e1b.b << std::endl;

		std::cout << "\t\t Entity 2 ComponentA should have the values 2.1, 2.2, 2.3, has: ";
		ComponentA e2a = e.getComponentCopyFromEntity<ComponentA>(e2);
		ComponentB e2b = e.getComponentCopyFromEntity<ComponentB>(e2);
		std::cout << e2a.x << ", " << e2a.y << ", " << e2a.z << ", \n\t\t\tComponentB should have value 0, has: " << e2b.b << std::endl;

		std::cout << "\t\t Entity 3 ComponentA should have the values 4.1, 4.2, 4.3, has: ";
		ComponentA e3a = e.getComponentCopyFromEntity<ComponentA>(e3);
		ComponentB e3b = e.getComponentCopyFromEntity<ComponentB>(e3);
		std::cout << e3a.x << ", " << e3a.y << ", " << e3a.z << ", \n\t\t\tComponentB should have value 1, has: " << e3b.b << std::endl;

		std::cout << "\t\t Entity 4 ComponentA should have the values 4.1, 4.2, 4.3, has: ";
		ComponentA e4a = e.getComponentCopyFromEntity<ComponentA>(e4);
		ComponentB e4b = e.getComponentCopyFromEntity<ComponentB>(e4);
		std::cout << e4a.x << ", " << e4a.y << ", " << e4a.z << ", \n\t\t\tComponentB should have value 0, has: " << e4b.b << std::endl;

		std::cout << "\t Testing registerSystem" << std::endl;
		e.registerSystem<System_Example>();
		std::cout << "\t\t Registered System_Example, sizeSystems should be 1, is: " << e.sizeSystems() << std::endl;

		std::cout << "\t Testing update" << std::endl;
		e.update(2.0f);
		std::cout << "\t\t Entity 1 ComponentA should have the values 4.1, 4.2, 4.3, has: ";
		e1a = e.getComponentCopyFromEntity<ComponentA>(e1);
		e1b = e.getComponentCopyFromEntity<ComponentB>(e1);
		std::cout << e1a.x << ", " << e1a.y << ", " << e1a.z << ", \n\t\t\tComponentB should have value 1, has: " << e1b.b << std::endl;

		std::cout << "\t\t Entity 2 ComponentA should have the values 2.1, 2.2, 2.3, has: ";
		e2a = e.getComponentCopyFromEntity<ComponentA>(e2);
		e2b = e.getComponentCopyFromEntity<ComponentB>(e2);
		std::cout << e2a.x << ", " << e2a.y << ", " << e2a.z << ", \n\t\t\tComponentB should have value 0, has: " << e2b.b << std::endl;

		std::cout << "\t\t Entity 3 ComponentA should have the values 6.1, 6.2, 6.3, has: ";
		e3a = e.getComponentCopyFromEntity<ComponentA>(e3);
		e3b = e.getComponentCopyFromEntity<ComponentB>(e3);
		std::cout << e3a.x << ", " << e3a.y << ", " << e3a.z << ", \n\t\t\tComponentB should have value 1, has: " << e3b.b << std::endl;

		std::cout << "\t\t Entity 4 ComponentA should have the values 4.1, 4.2, 4.3, has: ";
		e4a = e.getComponentCopyFromEntity<ComponentA>(e4);
		e4b = e.getComponentCopyFromEntity<ComponentB>(e4);
		std::cout << e4a.x << ", " << e4a.y << ", " << e4a.z << ", \n\t\t\tComponentB should have value 0, has: " << e4b.b << std::endl;
	}
}
#endif

#if RUN_MEMORY_LEAK_TEST
system("PAUSE"); // Snapshot
{
	size_t iterations = 100;
	size_t componentsPerIteration = 1000;
	size_t systemRegistersPerIteration = 100;
	for (size_t i = 0; i < iterations; i++)
	{
		EntityManager e;
		for (size_t j = 0; j < componentsPerIteration; j++)
		{
			e.newEntity(ComponentA{(float)(j), (float)(j + 1), (float)(j + 2)});
			e.newEntity(ComponentB{ (j % 2 == 0)?(true):(false) });
		}
		for (size_t j = 0; j < systemRegistersPerIteration; j++)
		{
			e.registerSystem<System_Example>();
		}
	}
}
system("PAUSE"); // Snapshot and compare Allocations with last snapshot. (Diff) should be 0.
#endif



system("PAUSE");
}