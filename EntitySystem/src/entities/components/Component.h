#pragma once
#include <typeinfo>

//! Define component bases

/* 
	For storing different types of components as pointers in one collection.
*/
struct ComponentBase
{
	virtual ~ComponentBase() { };
};

/*
	Wraps a global variable that ensures ComponentIdentifer has an unique identifier for each ComponentType.
*/
class ComponentIdentifierCounter
{
	template <typename ComponentType>
	friend class ComponentIdentifier;

	static size_t counter;
};

/*
	Used for setting and getting an unique identifier for every ComponentType.
	Uses ComponentIdentifierCounter to keep track of used identifiers.
*/
template <typename ComponentType>
class ComponentIdentifier : public ComponentBase
{
	static_assert(std::is_base_of<ComponentBase, ComponentType>::value, "ComponentType of ComponentIdentifier must be derived from ComponentBase");
private:
	static size_t identifier;
public:
	static size_t getIdentifier() 
	{ 
		if (identifier)
			return identifier;
		else
			return identifier = ComponentIdentifierCounter::counter++;
	};

};
template <typename ComponentType>
size_t ComponentIdentifier<ComponentType>::identifier = 0;

//! Define components

/*
	A vector component used for testing purposes.
*/
struct ComponentA : public ComponentBase
{
	float x, y, z;
	ComponentA(float x, float y, float z) : x(x), y(y), z(z) {};
};

/*
	A bool component used for testing purposes.
*/
struct ComponentB : public ComponentBase
{
	bool b;
	ComponentB(bool b) : b(b) {};
};