//
// Created by amit on 4/21/2022.
//
#pragma once
#include <chrono>

class CustomRandomEngine
{
private:
	static const uint_fast64_t a{ 0x5DEECE66DULL };
	static const uint_fast64_t c{ 0xBULL };
	static const uint_fast64_t m{ (1ULL << 48) - 1 };
	static uint_fast64_t seed;
public:
	CustomRandomEngine();
	uint_fast64_t getNext();
};
