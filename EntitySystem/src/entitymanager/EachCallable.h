#pragma once
#include <vector>
#include "units/UnitGroup.h"

class EntityManager;

///*
//	UnitTypes are mandatory by default, that is, they need to be present for EachCallable.eachCall to be called with each type.
//	When a UnitType is set to optional, EachCallable.eachCall will be called with an entity, regardless if the optional type was found for the
//	entity or not.
//*/
//template <bool... UnitsOptional>
//class OptionalUnits
//{
//private:
//	static std::vector<bool> optionalUnits;
//
//	/*
//		Unpacks parameter pack of bools, base case.
//	*/
//	template <bool UnitOptional>
//	static void unpackOptionalUnitsHelper(std::vector<bool>& optionalUnits)
//	{
//		optionalUnits.push_back(UnitOptional);
//	}
//
//	/*
//		Unpacks parameter pack of bools, recursive case.
//	*/
//	template <bool UnitOptional, bool F, bool... UnitsOptional>
//	static void unpackOptionalUnitsHelper(std::vector<bool>& optionalUnits)
//	{
//		optionalUnits.push_back(UnitOptional);
//		unpackOptionalUnitsHelper<F, UnitsOptional...>(optionalUnits);
//	}
//
//	/*
//		Unpacks parameter pack of bools.
//	*/
//	static std::vector<bool> unpackOptionalUnits()
//	{
//		std::vector<bool> optionalUnits;
//
//		if constexpr (sizeof...(UnitsOptional) != 0)
//			unpackOptionalUnitsHelper<UnitsOptional...>(optionalUnits);
//
//		return optionalUnits;
//	}
//public:
//	static std::vector<bool> getOptionalUnits()
//	{
//		return optionalUnits;
//	}
//};
//template <bool... UnitsOptional>
//std::vector<bool> OptionalUnits<UnitsOptional...>::optionalUnits = OptionalUnits<UnitsOptional...>::unpackOptionalUnits();
//
//
///*
//
//*/
//template <typename... UnitTypes>
//class Units
//{
//private:
//	/*
//		Identifiers of every type of unit the system should act on.
//	*/
//	static std::vector<size_t> unitIdentifiers;
//
//	/*
//		Unpacks parameter pack of unit types, base case.
//	*/
//	template <typename UnitType>
//	static void unpackUnitTypesHelper(std::vector<size_t>& unitIdentifiersUnpacking)
//	{
//		static_assert(std::is_base_of<Unit, UnitType>::value && std::is_base_of<UnitTypeIdentifier<UnitType>, UnitType>::value, "UnitType must be derived from Unit and UnitTypeIdentifier!");
//
//		unitIdentifiersUnpacking.push_back(UnitType::getIdentifier());
//	}
//
//	/*
//		Unpacks parameter pack of unit types, recursive case.
//	*/
//	template <typename UnitType, typename F, typename... Rest>
//	static void unpackUnitTypesHelper(std::vector<size_t>& unitIdentifiersUnpacking)
//	{
//		static_assert(std::is_base_of<Unit, UnitType>::value && std::is_base_of<UnitTypeIdentifier<UnitType>, UnitType>::value, "UnitType must be derived from Unit and UnitTypeIdentifier!");
//
//		unitIdentifiersUnpacking.push_back(UnitType::getIdentifier());
//		unpackUnitTypesHelper<F, Rest...>(unitIdentifiersUnpacking);
//	}
//
//	/*
//		Unpacks parameter pack of unit types.
//	*/
//	static std::vector<size_t> unpackUnitTypes()
//	{
//		std::vector<size_t> unitIdentifiersUnpacking;
//		unpackUnitTypesHelper<UnitTypes...>(unitIdentifiersUnpacking);
//
//		return unitIdentifiersUnpacking;
//	}
//public:
//	static std::vector<size_t> getUnitIdentifiers() { return unitIdentifiers; }
//};
//template <typename... UnitTypes>
//std::vector<size_t> Units<UnitTypes...>::unitIdentifiers = Units<UnitTypes...>::unpackUnitTypes();

/*
	Gives EntityManager an interface to submit unitGroups.
*/
class EachCallableBase
{
	friend class EntityManager;
protected:
	virtual void eachCall(UnitGroup& unitgroup) = 0;
public:
	virtual std::vector<size_t> getUnitIdentifiers() const = 0;
	virtual std::vector<bool> getOptionalUnits() const = 0;
};

template <typename... UnitTypes>
class EachCallable : public EachCallableBase
{
	//static_assert(std::is_base_of<Units, UnitsClass>::value, "UnitsClass must be derived from Units!");
	//static_assert(std::is_base_of<OptionalUnitsClass, OptionalUnits>::value, "OptionalUnitsClass must be derived from OptionalUnits!");

	/*
		Identifiers of every type of unit the system should act on, and whether they are optional.
	*/
	static std::vector<std::pair<size_t, bool>> unitIdentifiers;


	/*
		Unpacks parameter pack of unit types, base case.
	*/
	template <typename UnitType>
	static void unpackUnitTypesHelper(std::vector<size_t>& unitIdentifiersUnpacking)
	{
		static_assert(std::is_base_of<Unit, UnitType>::value && std::is_base_of<UnitTypeIdentifier<UnitType>, UnitType>::value, "UnitType must be derived from Unit and UnitTypeIdentifier!");

		unitIdentifiersUnpacking.push_back(UnitType::getIdentifier());
	}

	/*
		Unpacks parameter pack of unit types, recursive case.
	*/
	template <typename UnitType, typename F, typename... Rest>
	static void unpackUnitTypesHelper(std::vector<size_t>& unitIdentifiersUnpacking)
	{
		static_assert(std::is_base_of<Unit, UnitType>::value && std::is_base_of<UnitTypeIdentifier<UnitType>, UnitType>::value, "UnitType must be derived from Unit and UnitTypeIdentifier!");

		unitIdentifiersUnpacking.push_back(UnitType::getIdentifier());
		unpackUnitTypesHelper<F, Rest...>(unitIdentifiersUnpacking);
	}

	/*
		Unpacks parameter pack of unit types.
	*/
	static std::vector<size_t> unpackUnitTypes()
	{
		std::vector<size_t> unitIdentifiersUnpacking;
		unpackUnitTypesHelper<UnitTypes...>(unitIdentifiersUnpacking);

		return unitIdentifiersUnpacking;
	}

	//std::vector<size_t> getUnitIdentifiers() const override { return UnitsClass::getUnitIdentifiers(); }
	//std::vector<bool> getOptionalUnits() const override { return OptionalUnitsClass::getOptionalUnits(); }
};