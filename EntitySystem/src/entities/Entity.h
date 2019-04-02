#pragma once

// Just a container for entity id
struct Entity
{
	unsigned int id;

	bool operator==(const Entity& other) const
	{
		return id == other.id;
	}

	bool operator>(const Entity& other) const
	{
		return id > other.id;
	}
};