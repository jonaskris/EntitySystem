#pragma once
#include "Component.h"

/*
	Component used for testing purposes.
*/
struct ComponentA : public BasicComponent<ComponentA>, ComponentStorageSpecifier<ComponentA>
{
	float x, y, z;
	ComponentA(float x, float y, float z) : x(x), y(y), z(z) {};
};
size_t ComponentStorageSpecifier<ComponentA>::initialStorageCapacity = 1024;

/*
	Component used for testing purposes.
*/
struct ComponentB : public LimitedLifetimeComponent<ComponentB>
{
	bool b;
	ComponentB(bool b, const float& lifetime) : LimitedLifetimeComponent(lifetime), b(b) {};
};