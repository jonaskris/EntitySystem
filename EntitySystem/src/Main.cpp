#include <iostream>
#include <chrono>
#include <functional>
#include "entities/components/Components.h"
#include "entities/components/ComponentManager.h"
#include "entities/components/ComponentManagerIterator.h"
#include "entities/EntityManager.h"
#include "entities/Entity.h"
#include "systems/System.h"
#include "systems/Systems.h"

#define RUN_FUNCTION_TEST true
#define RUN_MEMORY_LEAK_TEST false
#define RUN_PERFORMANCE_TEST false


int main()
{
#if RUN_FUNCTION_TEST
{
	{
		ComponentA a{ 0.0f, 1.0f, 2.0f };
		ComponentB b{ false };

		ComponentManagerBase* componentManagerA = new ComponentManager<ComponentA>();
		ComponentManagerBase* componentManagerB = new ComponentManager<ComponentB>();

	// Testing ComponentManager.
		std::cout << "Testing ComponentManager..." << std::endl;
		// Testing insertComponent and size.
		componentManagerA->insertComponent(Entity{ 1 }, a);
		componentManagerA->insertComponent(Entity{ 2 }, b); // Fails as type check comes out as false
		componentManagerA->insertComponent(Entity{ 3 }, a);
		componentManagerA->insertComponent(Entity{ 4 }, b); // Fails as type check comes out as false

		componentManagerB->insertComponent(Entity{ 1 }, b);
		componentManagerB->insertComponent(Entity{ 2 }, a); // Fails as type check comes out as false
		componentManagerB->insertComponent(Entity{ 3 }, b);
		componentManagerB->insertComponent(Entity{ 4 }, a); // Fails as type check comes out as false

		std::cout << "\t Testing insertComponent and size..." << std::endl;
		std::cout << "\t\t Size of manager A, should be 2, is: " << componentManagerA->size() << std::endl;
		std::cout << "\t\t Size of manager B, should be 2, is: " << componentManagerB->size() << std::endl << std::endl;

		// Testing storesComponentType.
		std::cout << "\t Testing storesComponentType...." << std::endl;
		std::cout << "\t\t Manager a storesComponentType A, should be True, is: " << (componentManagerA->storesComponentType<ComponentA>() ? ("True") : ("False")) << std::endl;
		std::cout << "\t\t Manager a storesComponentType B, should be False, is: " << (componentManagerA->storesComponentType<ComponentB>() ? ("True") : ("False")) << std::endl;
		std::cout << "\t\t Manager b storesComponentType B, should be True, is: " << (componentManagerB->storesComponentType<ComponentB>() ? ("True") : ("False")) << std::endl;
		std::cout << "\t\t Manager b storesComponentType A, should be False, is: " << (componentManagerB->storesComponentType<ComponentA>() ? ("True") : ("False")) << std::endl << std::endl;
		
		// Testing getIdentifier.
		std::cout << "\t Testing getIdentifier..." << std::endl;
		std::cout << "\t\t ManagerA getIdentifier, should be 1, is: " << componentManagerA->getIdentifier() << std::endl;
		std::cout << "\t\t ManagerB getIdentifier, should be 2, is: " << componentManagerB->getIdentifier() << std::endl << std::endl;
		
		// Adding more components.
		componentManagerA->insertComponent(Entity{ 2 }, a);
		componentManagerA->insertComponent(Entity{ 4 }, a);
		componentManagerA->insertComponent(Entity{ 5 }, a);
		
		// Testing eraseComponentOf.
		std::cout << "\t Testing eraseComponentOf..." << std::endl;
			// Erasing non-existant component below minimum.
		bool erased1 = componentManagerA->eraseComponentOf(Entity{0});
		std::cout << "\t\t Tried erasing non-existant component below minimum, size should be 5, is: " << componentManagerA->size() << ", \n\t\t\treturn value should be False, is: " << ((erased1) ? ("True") : ("False")) << std::endl;
			// Erasing non-existant component above minimum.
		bool erased2 = componentManagerA->eraseComponentOf(Entity{6});
		std::cout << "\t\t Tried erasing non-existant component above minimum, size should be 5, is: " << componentManagerA->size() << ", \n\t\t\treturn value should be False, is: " << ((erased2) ? ("True") : ("False")) << std::endl;
			// Erasing existant component at minimum.
		bool erased3 = componentManagerA->eraseComponentOf(Entity{1});
		std::cout << "\t\t Tried erasing existing component at minimum, size should be 4, is: " << componentManagerA->size() << ", \n\t\t\treturn value should be True, is: " << ((erased3) ? ("True") : ("False")) << std::endl;
			// Erasing existant component at maximum.
		bool erased4 = componentManagerA->eraseComponentOf(Entity{5});
		std::cout << "\t\t Tried erasing existing component at maximum, size should be 3, is: " << componentManagerA->size() << ", \n\t\t\treturn value should be True, is: " << ((erased4) ? ("True") : ("False")) << std::endl;
			// Erasing existant component somewhere in middle.
		bool erased5 = componentManagerA->eraseComponentOf(Entity{4});
		std::cout << "\t\t Tried erasing existing component at maximum, size should be 2, is: " << componentManagerA->size() << ", \n\t\t\treturn value should be True, is: " << ((erased5) ? ("True") : ("False")) << std::endl;

		delete componentManagerA;
		delete componentManagerB;
	}
	// Testing EntityManager...
	std::cout << "Testing EntityManager..." << std::endl;
	{
		// Testing newEntity(components).
		std::cout << "\t Testing newEntity(components) and sizeEntities..." << std::endl;

		EntityManager e;
		Entity e1 = e.newEntity(ComponentA{ 1.1f, 1.2f, 1.3f }, ComponentB{ true });
		Entity e2 = e.newEntity(ComponentA{ 2.1f, 2.2f, 2.3f }, ComponentB{ false });
		std::cout << "\t\t Size should be 2, is: " << e.sizeEntities() << std::endl << std::endl;

		// Testing newEntity(void).
		std::cout << "\t Testing newEntity(void) and sizeEntities..." << std::endl;
		Entity e3 = e.newEntity();
		Entity e4 = e.newEntity();
		std::cout << "\t\t Size should be 4, is: " << e.sizeEntities() << std::endl << std::endl;

		// Testing newComponent and sizeEntities.
		std::cout << "\t Testing newComponent and sizeEntities..." << std::endl;
		e.newComponent(e3, ComponentA{ 3.1f, 3.2f, 3.3f });
		e.newComponent(e3, ComponentB{ true });
		e.newComponent(e4, ComponentA{ 4.1f, 4.2f, 4.3f });
		e.newComponent(e4, ComponentB{ false });

		std::cout << "\t\t Size should remain 4, is: " << e.sizeEntities() << std::endl << std::endl;

		// Testing each.
		std::cout << "\t Testing each..." << std::endl;

		// Increments x, y and z in ComponentA if boolean in ComponentB is true.
		e.each<ComponentA, ComponentB>(
			[&](std::vector<ComponentStoreType*> components) -> void
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
		ComponentA* e1a = e.getComponentFromEntity<ComponentA>(e1);
		ComponentB* e1b = e.getComponentFromEntity<ComponentB>(e1);
		std::cout << e1a->x << ", " << e1a->y << ", " << e1a->z << ", \n\t\t\tComponentB should have value 1, has: " << e1b->b << std::endl;

		std::cout << "\t\t Entity 2 ComponentA should have the values 2.1, 2.2, 2.3, has: ";
		ComponentA* e2a = e.getComponentFromEntity<ComponentA>(e2);
		ComponentB* e2b = e.getComponentFromEntity<ComponentB>(e2);
		std::cout << e2a->x << ", " << e2a->y << ", " << e2a->z << ", \n\t\t\tComponentB should have value 0, has: " << e2b->b << std::endl;

		std::cout << "\t\t Entity 3 ComponentA should have the values 4.1, 4.2, 4.3, has: ";
		ComponentA* e3a = e.getComponentFromEntity<ComponentA>(e3);
		ComponentB* e3b = e.getComponentFromEntity<ComponentB>(e3);
		std::cout << e3a->x << ", " << e3a->y << ", " << e3a->z << ", \n\t\t\tComponentB should have value 1, has: " << e3b->b << std::endl;

		std::cout << "\t\t Entity 4 ComponentA should have the values 4.1, 4.2, 4.3, has: ";
		ComponentA* e4a = e.getComponentFromEntity<ComponentA>(e4);
		ComponentB* e4b = e.getComponentFromEntity<ComponentB>(e4);
		std::cout << e4a->x << ", " << e4a->y << ", " << e4a->z << ", \n\t\t\tComponentB should have value 0, has: " << e4b->b << std::endl << std::endl;

		// Testing registerSystem.
		std::cout << "\t Testing registerSystem..." << std::endl;
		bool s1 = e.registerSystem<System_Example>();
		std::cout << "\t\t Registered System_Example, sizeSystems should be 1, is: " << e.sizeSystems() << "\n\t\t\t Should return True, returned: " << ((s1) ? ("True") : ("False")) << std::endl;
		bool s2 = e.registerSystem<System_Example>();
		std::cout << "\t\t Tried registering System_Example again, sizeSystems should remain 1, is: " << e.sizeSystems() << "\n\t\t\t Should return False, returned: " << ((s2) ? ("True") : ("False")) << std::endl << std::endl;
		// Testing update.
		std::cout << "\t Testing update..." << std::endl;
		e.update(2.0f);
		std::cout << "\t\t Entity 1 ComponentA should have the values 4.1, 4.2, 4.3, has: ";
		e1a = e.getComponentFromEntity<ComponentA>(e1);
		e1b = e.getComponentFromEntity<ComponentB>(e1);
		std::cout << e1a->x << ", " << e1a->y << ", " << e1a->z << ", \n\t\t\tComponentB should have value 1, has: " << e1b->b << std::endl;

		std::cout << "\t\t Entity 2 ComponentA should have the values 2.1, 2.2, 2.3, has: ";
		e2a = e.getComponentFromEntity<ComponentA>(e2);
		e2b = e.getComponentFromEntity<ComponentB>(e2);
		std::cout << e2a->x << ", " << e2a->y << ", " << e2a->z << ", \n\t\t\tComponentB should have value 0, has: " << e2b->b << std::endl;

		std::cout << "\t\t Entity 3 ComponentA should have the values 6.1, 6.2, 6.3, has: ";
		e3a = e.getComponentFromEntity<ComponentA>(e3);
		e3b = e.getComponentFromEntity<ComponentB>(e3);
		std::cout << e3a->x << ", " << e3a->y << ", " << e3a->z << ", \n\t\t\tComponentB should have value 1, has: " << e3b->b << std::endl;

		std::cout << "\t\t Entity 4 ComponentA should have the values 4.1, 4.2, 4.3, has: ";
		e4a = e.getComponentFromEntity<ComponentA>(e4);
		e4b = e.getComponentFromEntity<ComponentB>(e4);
		std::cout << e4a->x << ", " << e4a->y << ", " << e4a->z << ", \n\t\t\tComponentB should have value 0, has: " << e4b->b << std::endl << std::endl;

		Entity e5 = e.newEntity(ComponentB{ true });
		Entity e6 = e.newEntity(ComponentA{ 1.0f, 2.0f, 3.0f });

		std::cout << "\t Testing eraseEntity and sizeComponentManager" << std::endl; 
		std::cout << "\t\t Size of componentManagerA before any erasing should be 5, is: " << e.sizeComponentManager<ComponentA>() << "\n\t\t Size of componentManagerB before any erasing should be 5, is: " << e.sizeComponentManager<ComponentB>() << std::endl;
		e.eraseEntity(Entity{ 4 });
		std::cout << "\t\t Tried erasing entity 4, \n\t\t\t componentManagerA size should be 4, is: " << e.sizeComponentManager<ComponentA>() << "\n\t\t\t componentManagerB size should be 4, is: " << e.sizeComponentManager<ComponentB>() << std::endl;
		e.eraseEntity(Entity{ 5 });
		std::cout << "\t\t Tried erasing entity 5, \n\t\t\t componentManagerA size should be 4, is: " << e.sizeComponentManager<ComponentA>() << "\n\t\t\t componentManagerB size should be 3, is: " << e.sizeComponentManager<ComponentB>() << std::endl;
		e.eraseEntity(Entity{ 6 });
		std::cout << "\t\t Tried erasing entity 6, \n\t\t\t componentManagerA size should be 3, is: " << e.sizeComponentManager<ComponentA>() << "\n\t\t\t componentManagerB size should be 3, is: " << e.sizeComponentManager<ComponentB>() << std::endl;
	}
}
#endif

#if RUN_MEMORY_LEAK_TEST
system("PAUSE"); // Snapshot
{
	size_t iterations = 100;
	size_t componentsPerIteration = 1000;
	size_t updatesPerIteration = 100;
	for (size_t i = 0; i < iterations; i++)
	{
		EntityManager e;
		for (size_t j = 0; j < componentsPerIteration; j++)
		{
			e.newEntity(ComponentA{(float)(j), (float)(j + 1), (float)(j + 2)});
			e.newEntity(ComponentB{ (j % 2 == 0)?(true):(false) });
		}
		
		e.registerSystem<System_Example>();	
	}
}
system("PAUSE"); // Snapshot and compare Allocations with last snapshot. (Diff) should be 0.
#endif

#if RUN_PERFORMANCE_TEST
{
	std::cout << "Running performance tests with one system" << std::endl;
	std::function<void(size_t, size_t)> testLambda = [](size_t entitiesCount, size_t duration) 
	{
		EntityManager e;
		e.registerSystem<System_Example>();

		for (size_t j = 0; j < entitiesCount; j++)
		{
			e.newEntity(ComponentA{ (float)(j), (float)(j + 1), (float)(j + 2) });
			e.newEntity(ComponentB{ (j % 2 == 0) ? (true) : (false) });
		}

		auto start = std::chrono::system_clock::now();
		size_t count = 0;
		do
		{
			count++;
			e.update(1.0f);
		} while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() < duration);

		std::cout << "Performance test with " << entitiesCount << " entities, average updates per second: " << count / duration << std::endl;
	};

	size_t duration = 5;
	testLambda(100, duration);
	testLambda(1000, duration);
	testLambda(10000, duration);

 }
#endif



system("PAUSE");
}