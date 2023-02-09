#pragma once
#include <cstdint>

class CustomRandomEngine
{
	static constexpr uint64_t A{0x5DEECE66DULL};
	static constexpr uint64_t C{0xBULL};

public:
	static constexpr uint64_t M{(1ULL << 48) - 1};
	uint64_t Seed;
	CustomRandomEngine();
	explicit CustomRandomEngine(uint64_t seed);
	uint64_t GetNext();
	template <class T>
	T GetNext(T maxValue);
	template <class T>
	T GetNext(T minValue, T maxValue);
	float GetNextFloat();
};


template <typename T>
T CustomRandomEngine::GetNext(T maxValue)
{
	return GetNext<T>(0, maxValue);
}

template <typename T>
T CustomRandomEngine::GetNext(T minValue, T maxValue)
{
	T modulo = maxValue - minValue;
	T next = GetNext() % modulo;
	return next + minValue;
}