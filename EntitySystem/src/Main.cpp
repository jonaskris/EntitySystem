#include <iostream>
#include <chrono>
#include "entities/components/Component.h"
#include "entities/components/ComponentManager.h"
#include "entities/components/ComponentManagerIterator.h"
#include "entities/Entity.h"

#define RUN_TESTS true

int main()
{
	#if RUN_TESTS
		ComponentA a{0.0f, 1.0f, 2.0f};
		ComponentB b{false};
	
		ComponentManagerBase* componentManagerA = new ComponentManager<ComponentA>(1024);
		ComponentManagerBase* componentManagerB = new ComponentManager<ComponentB>(1024);
	
// Testing ComponentManager
		std::cout << "Testing ComponentManager..." << std::endl;
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
	
	
			std::cout << "\t ManagerA storesComponentType A, should be True, is: " << (componentManagerA->storesComponentType<ComponentA>() ? ("True") : ("False")) << std::endl;
			std::cout << "\t ManagerA storesComponentType B, should be False, is: " << (componentManagerA->storesComponentType<ComponentB>() ? ("True") : ("False")) << std::endl;
			std::cout << "\t ManagerB storesComponentType B, should be True, is: " << (componentManagerB->storesComponentType<ComponentB>() ? ("True") : ("False")) << std::endl;
			std::cout << "\t ManagerB storesComponentType A, should be False, is: " << (componentManagerB->storesComponentType<ComponentA>() ? ("True") : ("False")) << std::endl << std::endl;
	
			std::cout << "\t Size of ManagerA, should be 3, is: " << componentManagerA->size() << std::endl;
			std::cout << "\t Size of ManagerB, should be 4, is: " << componentManagerB->size() << std::endl << std::endl;
	
		// Comparing speed of insertComponent and safeInsertComponent
#ifdef _DEBUG
			std::cout << "\t Speed comparison of insertComponent and safeInsertComponent are disabled in debug mode, as it yields inaccurate results." << std::endl;
#else
			std::cout << "\t Comparing speed of insertComponent and safeInsertComponent..." << std::endl;
			int countTests = 1000;
			int componentsInsertedPerTest = 1024;
			double avg = 0;
			for (size_t i = 0; i < countTests; i++) {
				delete componentManagerA;
				componentManagerA = new ComponentManager<ComponentA>(componentsInsertedPerTest);

				auto s1 = std::chrono::high_resolution_clock::now();
				for (size_t i = 0; i < componentsInsertedPerTest; i++)
				{
					componentManagerA->insertComponent(Entity{ i }, &ComponentA{ i*1.0f, i*2.0f, i*3.0f });
				}
				auto e1 = std::chrono::high_resolution_clock::now();

				long nano1 = std::chrono::duration_cast<std::chrono::nanoseconds>(e1 - s1).count();

				delete componentManagerA;
				componentManagerA = new ComponentManager<ComponentA>(componentsInsertedPerTest);

				auto s2 = std::chrono::high_resolution_clock::now();
				for (size_t i = 0; i < componentsInsertedPerTest; i++)
				{
					componentManagerA->safeInsertComponent(Entity{ i }, &ComponentA{ i*1.0f, i*2.0f, i*3.0f });
				}
				auto e2 = std::chrono::high_resolution_clock::now();

				long nano2 = std::chrono::duration_cast<std::chrono::nanoseconds>(e2 - s2).count();

				avg += ((((float)nano2 / (float)nano1) * 100) - 100) / countTests;
			}
			std::cout << "\t\t safeInsertComponent is on average " << avg << "% slower than insertComponent " << std::endl;
#endif
		// Testing ComponentManagerIterator...
		std::cout << "Testing ComponentManagerIterator..." << std::endl;
			ComponentManager<ComponentA> ca(1024);
			ComponentManager<ComponentB> cb(1024);

			ca.insertComponent(Entity{ 1 }, &ComponentA{ 1.0f, 2.0f, 3.0f });
			ca.insertComponent(Entity{ 2 }, &ComponentA{ 2.0f, 3.0f, 4.0f });
			ca.insertComponent(Entity{ 3 }, &ComponentA{ 3.0f, 4.0f, 5.0f });

			cb.insertComponent(Entity{ 3 }, &ComponentB{ true });
			cb.insertComponent(Entity{ 2 }, &ComponentB{ false });
			cb.insertComponent(Entity{ 3 }, &ComponentB{ true });

			ComponentManagerIterator ia = ca.begin();
			ComponentManagerIterator ib = cb.begin();

			std::cout << "\t Testing ComponentManagerIterator.sameEntity(ComponentManagerIterator), should be False, is: " << (ia.sameEntity(ib) ? ("True") : ("False")) << std::endl; // False

			ia.increment();
			ib.increment();

			std::cout << "\t Testing ComponentManagerIterator.sameEntity(ComponentManagerIterator), should be True, is: " << (ia.sameEntity(ib) ? ("True") : ("False")) << std::endl; // True

			ia.increment();

			std::cout << "\t Testing ComponentManagerIterator.sameEntity(ComponentManagerIterator), should be False, is: " << (ia.sameEntity(ib) ? ("True") : ("False")) << std::endl; // False

			ib.increment();

			std::cout << "\t Testing ComponentManagerIterator.sameEntity(ComponentManagerIterator), should be True, is: " << (ia.sameEntity(ib) ? ("True") : ("False")) << std::endl << std::endl; // True

#endif
	system("PAUSE");
}