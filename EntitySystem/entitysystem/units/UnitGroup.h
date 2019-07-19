#pragma once
#include <map>
#include <utility>
#include "Unit.h"
#include "../utils/Algorithm.h"

namespace entitysystem
{
	class EntityManager;

	/*
		UnitGroups are used to pass around groups of Units regardless of UnitType.
	*/
	class UnitGroup
	{
		friend class EntityManager;

		struct Group
		{
			size_t unitIdentifier;

			UnitBase* first;
			size_t count;

			Group(const size_t& unitIdentifier, UnitBase* const first, const size_t& count) : unitIdentifier(unitIdentifier), first(first), count(count) {};

			operator size_t() const { return unitIdentifier; }
		};
	private:
		std::vector<Group> groups;
	public:
		void insert(const size_t& unitIdentifier, const std::pair<UnitBase*, size_t>& group)
		{
			insertSorted(groups, Group{ unitIdentifier, group.first, group.second });
		}

		void insert(const size_t& unitIdentifier, UnitBase* const first, const size_t& count)
		{
			insertSorted(groups, Group{ unitIdentifier, first, count });
		}

		template <typename UnitType>
		std::pair<UnitType*, size_t> get()
		{
			static_assert(std::is_base_of<UnitBase, UnitType>::value, "UnitType must be derived from Unit!");

			int index = binarySearch(groups, UnitType::getIdentifier(), 0, groups.size());

			if (index < 0)
				return std::pair(static_cast<UnitType*>(nullptr), 0);

			return std::pair(static_cast<UnitType*>(groups.at(index).first), groups.at(index).count);
		}
	};
}