#pragma once
#include <typeinfo>

//! Define component bases
// For storing different types of components as pointers in one collection.
struct ComponentBase
{
	virtual ~ComponentBase() { };
};

//! Define components
struct ComponentA : public ComponentBase
{
	float x, y, z;
	ComponentA(float x, float y, float z) : x(x), y(y), z(z) {};
};

struct ComponentB : public ComponentBase
{
	bool b;
	ComponentB(bool b) : b(b) {};
};