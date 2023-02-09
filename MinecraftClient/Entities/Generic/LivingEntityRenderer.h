#pragma once
#include "EntityRenderer.h"
#include "Util/States/LivingEntityState.h"

template <typename StateType> requires std::is_base_of_v<LivingEntityState, StateType>
class LivingEntityRenderer : public EntityRenderer<StateType>
{
protected:
    explicit LivingEntityRenderer(const StateType& state, const StateType& oldState) : EntityRenderer<StateType>(state, oldState)
    {
    }
};
