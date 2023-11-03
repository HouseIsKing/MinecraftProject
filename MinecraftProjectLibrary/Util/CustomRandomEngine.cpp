#include "CustomRandomEngine.h"
#include <chrono>

CustomRandomEngine::CustomRandomEngine() : Seed(static_cast<uint_fast64_t>(std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::milliseconds(1)))
{
}

CustomRandomEngine::CustomRandomEngine(const uint64_t seed) : Seed(seed)
{
}

uint_fast64_t CustomRandomEngine::GetNext()
{
	Seed = (Seed * A + C) % M;
	return Seed;
}

float CustomRandomEngine::GetNextFloat()
{
	return static_cast<float>(static_cast<double>(GetNext()) / M);
}