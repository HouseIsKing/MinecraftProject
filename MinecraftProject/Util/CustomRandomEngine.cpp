//
// Created by amit on 4/21/2022.
//

#include "CustomRandomEngine.h"

uint_fast64_t CustomRandomEngine::seed{ (uint_fast64_t)(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1)) };

CustomRandomEngine::CustomRandomEngine()
{
}

uint_fast64_t CustomRandomEngine::getNext()
{
	seed = (seed * a + c) % m;
	return seed;
}
