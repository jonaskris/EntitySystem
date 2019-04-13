#pragma once

/*
	Entities are unique per EntityManager,
	and can only be assigned one component per ComponentType.
*/
struct Entity
{
	const size_t id;

	Entity(const size_t& id) : id(id) {};

	Entity operator=(const Entity& other) {
		return Entity{ id };
	}

	bool operator==(const Entity& other) const
	{
		return id == other.id;
	}

	bool operator>(const Entity& other) const
	{
		return id > other.id;
	}

	bool operator<(const Entity& other) const
	{
		return id < other.id;
	}
};