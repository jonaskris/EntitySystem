#include <iostream>
#include <chrono>
#include <functional>
#include "entities/components/Components.h"
#include "entities/components/Component.h"
#include "entities/components/ComponentManager.h"
#include "entities/components/ComponentManagerIterator.h"
#include "entities/EntityManager.h"
#include "entities/Entity.h"
#include "systems/System.h"
#include "systems/Systems.h"

#define RUN_FUNCTION_TEST false
#define RUN_MEMORY_LEAK_TEST false
#define RUN_PERFORMANCE_TEST false


int main()
{
#if RUN_FUNCTION_TEST
{
	// Testing EntityManager...
	std::cout << "Testing EntityManager..." << std::endl;
	{
		// Testing newEntity(components).
		std::cout << "\t Testing newEntity(components) and sizeEntities..." << std::endl;

		EntityManager e;
		Entity e1 = e.newEntity(ComponentA{ 1.1f, 1.2f, 1.3f }, ComponentB{ true, 10.0f });
		Entity e2 = e.newEntity(ComponentA{ 2.1f, 2.2f, 2.3f }, ComponentB{ false, 10.0f });
		std::cout << "\t\t Size should be 2, is: " << e.sizeEntities() << std::endl << std::endl;

		// Testing newEntity(void).
		std::cout << "\t Testing newEntity(void) and sizeEntities..." << std::endl;
		Entity e3 = e.newEntity();
		Entity e4 = e.newEntity();
		std::cout << "\t\t Size should be 4, is: " << e.sizeEntities() << std::endl << std::endl;

		// Testing newComponent and sizeEntities.
		std::cout << "\t Testing newComponent and sizeEntities..." << std::endl;
		e.newComponent(e3, ComponentA{ 3.1f, 3.2f, 3.3f });
		e.newComponent(e3, ComponentB{ true, 10.0f });
		e.newComponent(e4, ComponentA{ 4.1f, 4.2f, 4.3f });
		e.newComponent(e4, ComponentB{ false, 10.0f });

		std::cout << "\t\t Size should remain 4, is: " << e.sizeEntities() << std::endl << std::endl;

		std::cout << "\t Testing getComponentVectorByType..." << std::endl;
		std::vector<ComponentA>* vecA = e.getComponentVectorByType<ComponentA>();
		std::cout << "\t\t Got component vector of type ComponentA from EntityManager, size should be 4, is: " << vecA->size() << std::endl;
		std::vector<ComponentB>* vecB = e.getComponentVectorByType<ComponentB>();
		std::cout << "\t\t Got component vector of type ComponentB from EntityManager, size should be 4, is: " << vecB->size() << std::endl << std::endl;

		// Testing each.
		std::cout << "\t Testing each..." << std::endl;

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

		Entity e5 = e.newEntity(ComponentB{ true, 1.0f });
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

{



	// Tests
		// Component
	std::cout << "Component tests..." << std::endl;
			// Identifiers should be unique
	std::cout << "\tComponentA identifier should be 1, is: " << ComponentTypeIdentifier<ComponentA>::getIdentifierStatic() << std::endl;
	std::cout << "\tComponentB identifier should be 2, is: " << ComponentTypeIdentifier<ComponentB>::getIdentifierStatic() << std::endl;
			// Initial storage capacity should be correct
	std::cout << "\tComponentA initialStorageCapacity should be 1024, is: " << ComponentA::getInitialStorageCapacity() << std::endl << std::endl;
		// System
	std::cout << "System tests..." << std::endl;
			// Identifiers should be unique
	std::cout << "\tSystem_Example identifier should be 1, is: " << SystemIdentifier<System_Example>::getIdentifierStatic() << std::endl;
	std::cout << "\tSystem_Test identifier should be 2, is: " << SystemIdentifier<System_Test>::getIdentifierStatic() << std::endl << std::endl;
		// EntityManager
	{
		std::cout << "EntityManager tests..." << std::endl;
		EntityManager em;
		// System
			// SystemsSize
		std::cout << "\tSystemSize before registering system should be 0, is: " << em.sizeSystems() << std::endl;
		em.registerSystem(new System_Example());
		std::cout << "\tSystemSize after registering system should be 1, is: " << em.sizeSystems() << std::endl;
			// ComponentManager
				// ComponentManagersSize
					// Check that size changes appropriately with adding new components 
		std::cout << "\tSizeComponentManagers before creating any entity or component should be 0, is: " << em.sizeComponentManagers() << std::endl;
		em.newEntity(ComponentA{ 1.0f, 2.0f, 3.0f });
						// Should increase when a component is added which type is not added before
		std::cout << "\tSizeComponentManagers after creating an entity with one component should be 1, is: " << em.sizeComponentManagers() << std::endl;
		em.newEntity(ComponentB{ false, 10.0f });
		std::cout << "\tSizeComponentManagers after creating an entity with another component should be 2, is: " << em.sizeComponentManagers() << std::endl;
						// Should NOT increase when a component is added which type IS added before
		em.newEntity(ComponentB{ false, 15.0f });
		std::cout << "\tSizeComponentManagers after creating an entity of already added type should remain 2, is: " << em.sizeComponentManagers() << std::endl;
				// getComponentVectorByType
		std::cout << "\tGetComponentVectorByType<ComponentA> should return a vector of size 1, is: " << em.sizeComponentManager<ComponentA>() << std::endl;
		std::cout << "\tGetComponentVectorByType<ComponentB> should return a vector of size 2, is: " << em.sizeComponentManager<ComponentB>() << std::endl;
			// DisableEntity
				// Appropriate components should be erased at the end of update
		em.update(9.0f);
		std::cout << "\tLimitedLifetimeComponent should not be erased if not enough time has elapsed, SizeComponentManager<ComponentB> should remain 2, is: " << em.sizeComponentManager<ComponentB>() << std::endl;
		em.update(1.0f);
		std::cout << "\tLimitedLifetimeComponent should be erased if enough time has elapsed, SizeComponentManager<ComponentB> should be 1, is: " << em.sizeComponentManager<ComponentB>() << std::endl;
		em.update(6.0f);
		std::cout << "\tLimitedLifetimeComponent should be erased if enough time has elapsed, SizeComponentManager<ComponentB> should be 0, is: " << em.sizeComponentManager<ComponentB>() << std::endl;

		std::cout << "\tSizeComponentManager of BasicComponent ComponentA before disabling a component should be 1, is: " << em.sizeComponentManager<ComponentA>() << std::endl;
		em.disableComponentsOf(Entity{ 1 });
		std::cout << "\tSizeComponentManager of BasicComponent ComponentA AFTER disabling AND BEFORE UPDATING component should remain 1, is: " << em.sizeComponentManager<ComponentA>() << std::endl;
		em.update(0.0f);
		std::cout << "\tSizeComponentManager of BasicComponent ComponentA AFTER disabling AND AFTER UPDATING component should be 0, is: " << em.sizeComponentManager<ComponentA>() << std::endl << std::endl;
	}
			// Update
	{
		EntityManager em;
		em.registerSystem(new System_Example());
		em.newEntity(ComponentA{ 0.0f, 1.0f, 2.0f }, ComponentB{ true, 8.0f});
		em.newEntity(ComponentA{ 3.0f, 4.0f, 5.0f }, ComponentB{ false, 10.0f});
		em.newEntity(ComponentA{ 6.0f, 7.0f, 8.0f });
		em.newEntity(ComponentA{ 6.0f, 7.0f, 8.0f });
		em.newEntity(ComponentA{ 6.0f, 7.0f, 8.0f });

		std::cout << "\tBefore update, EntityManager ComponentManagerA/ComponentManagerB should contain components with values: " << std::endl;
		std::cout << "\tComponentA{ 0, 1, 2, false }, ComponentA{ 3, 4, 5, false }, ComponentA{ 6, 7, 8, false }" << std::endl;
		std::cout << "\tComponentB{ true, 8 }, ComponentB{ false, 10 }" << std::endl;
		std::cout << "\tContains: " << std::endl;
		std::vector<ComponentA>* aVec = em.getComponentVectorByType<ComponentA>();
		std::vector<ComponentB>* bVec = em.getComponentVectorByType<ComponentB>();
		std::cout << "\tComponentA{ " << aVec->at(0).x << ", " << aVec->at(0).y << ", " << aVec->at(0).z << ", " << ((aVec->at(0).getDisabled()) ? ("true") : ("false")) << " }" << ", ComponentA{ " << aVec->at(1).x << ", " << aVec->at(1).y << ", " << aVec->at(1).z << ", " << ((aVec->at(1).getDisabled()) ? ("true") : ("false")) << " }" << ", ComponentA{ " << aVec->at(2).x << ", " << aVec->at(2).y << ", " << aVec->at(2).z << ", " << ((aVec->at(2).getDisabled()) ? ("true") : ("false")) << " }" << std::endl;
		std::cout << "\tComponentB{ " << ((bVec->at(0).b)?("true"):("false")) << ", " << bVec->at(0).lifetime  << " }" << ", ComponentB{ " << ((bVec->at(1).b) ? ("true") : ("false")) << ", " << bVec->at(1).lifetime << " }" << std::endl;
		
		em.update(1.0f);

		std::cout << "\tAfter update, EntityManager ComponentManagerA/ComponentManagerB should contain components with values: " << std::endl;
		std::cout << "\tComponentA{ 1, 2, 3, false }, ComponentA{ 3, 4, 5, false }, ComponentA{ 6, 7, 8, false }" << std::endl;
		std::cout << "\tComponentB{ true, 7 }, ComponentB{ false, 9 }" << std::endl;
		std::cout << "\tContains: " << std::endl;
		aVec = em.getComponentVectorByType<ComponentA>();
		bVec = em.getComponentVectorByType<ComponentB>();
		std::cout << "\tComponentA{ " << aVec->at(0).x << ", " << aVec->at(0).y << ", " << aVec->at(0).z << ", " << ((aVec->at(0).getDisabled()) ? ("true") : ("false")) << " }" << ", ComponentA{ " << aVec->at(1).x << ", " << aVec->at(1).y << ", " << aVec->at(1).z << ", " << ((aVec->at(1).getDisabled()) ? ("true") : ("false")) << " }" << ", ComponentA{ " << aVec->at(2).x << ", " << aVec->at(2).y << ", " << aVec->at(2).z << ", " << ((aVec->at(2).getDisabled()) ? ("true") : ("false")) << " }" << std::endl;
		std::cout << "\tComponentB{ " << ((bVec->at(0).b) ? ("true") : ("false")) << ", " << bVec->at(0).lifetime << " }" << ", ComponentB{ " << ((bVec->at(1).b) ? ("true") : ("false")) << ", " << bVec->at(1).lifetime << " }" << std::endl;
	}
}
system("PAUSE");
}