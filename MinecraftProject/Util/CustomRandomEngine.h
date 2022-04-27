//
// Created by amit on 4/21/2022.
//
#pragma once
#include <chrono>

class CustomRandomEngine
{
private:
	static const uint64_t a{ 0x5DEECE66D };
	static const uint64_t c{ 0xBULL };
	static const uint64_t m{ 1ULL << 48 };
	static uint64_t seed;
public:
	CustomRandomEngine();
	uint64_t getNext();
};
