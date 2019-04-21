#pragma once
#include "Component.h"

/*
	Component used for testing purposes.
*/
struct ComponentA : public Component<ComponentA>
{
	float x, y, z;
	ComponentA(float x, float y, float z) : x(x), y(y), z(z) {};
};

/*
	Component used for testing purposes.
*/
struct ComponentB : public Component<ComponentB>
{
	bool b;
	explicit ComponentB(bool b) : b(b) {};
};