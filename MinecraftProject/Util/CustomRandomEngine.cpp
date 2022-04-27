//
// Created by amit on 4/21/2022.
//

#include "CustomRandomEngine.h"

uint64_t CustomRandomEngine::seed{ (uint64_t)(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1)) };

CustomRandomEngine::CustomRandomEngine()
{
}

uint64_t CustomRandomEngine::getNext()
{
	seed = (seed * a + c) % m;
	return seed;
}
