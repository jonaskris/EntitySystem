#include <iostream>
#include <chrono>
#include <functional>
#include "components/Components.h"
#include "events/Events.h"
#include "components/Component.h"
#include "units/UnitManager.h"
#include "units/UnitManagerIterator.h"
#include "entitymanager/EntityManager.h"
#include "systems/System.h"
#include "systems/Systems.h"

#define RUN_FUNCTION_TEST true
#define RUN_MEMORY_LEAK_TEST false
#define RUN_PERFORMANCE_TEST false

int main()
{
	using namespace entitysystem;
#if RUN_FUNCTION_TEST
{
		// Creating EntityManager...
		EntityManager em = EntityManager();

		// Registering systems...
		std::cout << "Registering systems..." << std::endl;
			std::cout << "\tEntityManager.sizeSystems() should be 0, is: " << em.sizeSystems() << std::endl;
		em.registerSystem(new SystemA());
			std::cout << "\tEntityManager.sizeSystems() should be 1, is: " << em.sizeSystems() << std::endl;
		em.registerSystem(new SystemB());
			std::cout << "\tEntityManager.sizeSystems() should be 2, is: " << em.sizeSystems() << std::endl;

		// Inserting and updating units...
		std::cout << "Inserting and updating units..." << std::endl;
			std::cout << "\tBefore inserting units..." << std::endl;
				std::cout << "\t\tEntityManager.sizeUnitsType<ComponentA>() should be 0, is: " << em.sizeUnitsType<ComponentA>() << std::endl;
				std::cout << "\t\tEntityManager.sizeUnitsType<EventA>() should be 0, is: " << em.sizeUnitsType<EventA>() << std::endl;
				std::cout << "\t\tEntityManager.sizeUnitsType<EventB>() should be 0, is: " << em.sizeUnitsType<EventB>() << std::endl;
				std::cout << "\t\tEntityManager.sizeUnits() should be 0, is: " << em.sizeUnits() << std::endl;

		em.newEntity(ComponentA{ 0.0f, 1.0f, 2.0f }, EventB { 2.0f, true}); // TargetedEvent EventB should have entityId set to the same entityId as ComponentA (A new entityId will be made).
		em.newUnit(ComponentA{ 3.0f, 4.0f, 5.0f });
		em.newUnit(EventA{ true, 1.0f });
			std::cout << "\tAfter inserting units..." << std::endl;
				std::cout << "\t\tEntityManager.sizeUnitsType<ComponentA>() should be 2, is: " << em.sizeUnitsType<ComponentA>() << std::endl;
				std::cout << "\t\tEntityManager.sizeUnitsType<EventA>() should be 1, is: " << em.sizeUnitsType<EventA>() << std::endl;
				std::cout << "\t\tEntityManager.sizeUnitsType<EventB>() should be 1, is: " << em.sizeUnitsType<EventB>() << std::endl;
				std::cout << "\t\tEntityManager.sizeUnits() should be 4, is: " << em.sizeUnits() << std::endl;

			em.insertFromQueues();
			std::vector<ComponentA>* componentVector = em.getUnitVector<ComponentA>();

			std::cout << "\tBefore update, components should have values: " << std::endl;
				std::cout << "\t\tComponentA{ 0, 1, 2 }, ComponentA{ 3, 4, 5 }" << std::endl;
				std::cout << "\t\tActual values: " << std::endl;
				std::cout << "\t\tComponentA{ " << componentVector->at(0).x << ", " << componentVector->at(0).y << ", " << componentVector->at(0).z << " }" << ", ComponentA{ " << componentVector->at(1).x << ", " << componentVector->at(1).y << ", " << componentVector->at(1).z << " }" << std::endl;

			em.update(1.0f);
			componentVector = em.getUnitVector<ComponentA>();

			std::cout << "\tAfter update, components should have values: " << std::endl;
				std::cout << "\t\tComponentA{ 2, 3, 4 }, ComponentA{ 4, 5, 6 }" << std::endl;
				std::cout << "\t\tActual values: " << std::endl;
				std::cout << "\t\tComponentA{ " << componentVector->at(0).x << ", " << componentVector->at(0).y << ", " << componentVector->at(0).z << " }" << ", ComponentA{ " << componentVector->at(1).x << ", " << componentVector->at(1).y << ", " << componentVector->at(1).z << " }" << std::endl;
}
#endif

#if RUN_MEMORY_LEAK_TEST
system("PAUSE"); // Snapshot
{
	size_t iterations = 100;
	size_t componentsPerIteration = 1000;
	for (size_t i = 0; i < iterations; i++)
	{
		EntityManager e;
		for (size_t j = 0; j < componentsPerIteration; j++)
		{
			e.newEntity(ComponentA{(float)(j), (float)(j + 1), (float)(j + 2)});
			e.newEntity(EventB{ 1.0f, (j % 2 == 0) ? (true) : (false) });
		}
		
		e.registerSystem(new SystemB());	
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
		e.registerSystem(new SystemB());

		for (size_t j = 0; j < entitiesCount; j++)
		{
			e.newEntity(ComponentA{ (float)(j), (float)(j + 1), (float)(j + 2) });
			e.newEntity(EventB{ 999999.0f, (j % 2 == 0) ? (true) : (false) });
		}

		auto start = std::chrono::system_clock::now();
		size_t count = 0;
		do
		{
			count++;
			e.update(0.01);
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
}
system("PAUSE");
}