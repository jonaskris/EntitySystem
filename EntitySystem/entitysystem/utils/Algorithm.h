#pragma once
#include <vector>
#include <utility>

namespace entitysystem
{
	/*
		Returns index of element in sorted vector equal to key using binary search.
	*/
	template <typename V, typename T>
	int binarySearch(const std::vector<V>& vec, const T& key, const int& low, const int& high)
	{
		if (low > high)
			return -1;

		int mid = (int)low + (((int)high - (int)low) >> 1);

		if (vec.at(mid) == key) return mid;
		if (vec.at(mid) > key)
			return binarySearch(vec, key, low, mid - 1);
		else
			return binarySearch(vec, key, mid + 1, high);
	}

	/*
		Returns indices span of elements in sorted vector equal to element at index.
	*/
	template <typename V>
	std::pair<size_t, size_t> getEqualNeighbours(const std::vector<V>& vec, const int& index)
	{
		if (index < 0)
			return std::pair(0, 0);

		V value = vec.at(index);

		int low = index;
		int high = index;

		while (!(low <= 0) && (vec.at(low - 1) == value))
			low--;

		while (!(high >= vec.size() - 1) && (vec.at(high + 1) == value))
			high++;

		return std::pair(low, high + 1);
	}


	/*
		Returns indices span of elements in sorted vector equal to key using binary search.
	*/
	template <typename V, typename T>
	std::pair<size_t, size_t> binarySearchGroup(const std::vector<V>& vec, const T& key, const size_t& low, const size_t& high)
	{
		return getEqualNeighbours(vec, binarySearch(vec, key, low, high));
	}

	template <typename V>
	void insertSorted(std::vector<V>& vec, const V& newElement)
	{
		bool inserted = false;
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			if (newElement < *it)
			{
				vec.insert(it, newElement);
				inserted = true;
				break;
			}
		}
		if (!inserted)
			vec.push_back(newElement);
	}

	template <typename V>
	void insertSortedReverse(std::vector<V>& vec, const V& newElement)
	{
		bool inserted = false;
		for (auto it = vec.rbegin(); it != vec.rend(); it++)
		{
			if (newElement > *it)
			{
				vec.insert(it.base(), newElement);
				inserted = true;
				break;
			}
		}
		if (!inserted)
			vec.push_back(newElement);
	}
}