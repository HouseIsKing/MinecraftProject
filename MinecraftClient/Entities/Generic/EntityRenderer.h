#pragma once
#include "Entities/Entity.h"
#include "Util/BoundingBox.h"
#include "Util/States/EntityState.h"
#include "Util/TessellationHelper.h"

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
class EntityRenderer
{
protected:
    const StateType& State;
    const StateType& OldState;
    TessellationHelper Tessellation;
    TransformStruct Transform;
    EntityRenderer(const StateType& state, const StateType& oldState);

public:
    virtual ~EntityRenderer() = default;
    EntityRenderer(const EntityRenderer&) = default;
    EntityRenderer& operator=(const EntityRenderer&) = default;
    EntityRenderer(EntityRenderer&&) noexcept = default;
    EntityRenderer& operator=(EntityRenderer&&) noexcept = default;
    void virtual Render(float partialTick);
    void virtual Changed();
};

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
EntityRenderer<StateType>::EntityRenderer(const StateType& state, const StateType& oldState) : State(state), OldState(oldState), Tessellation(&Transform), Transform(State.EntityTransform)
{
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityRenderer<StateType>::Render(float /*partialTick*/)
{
}

template <typename StateType> requires std::is_base_of_v<EntityState, StateType>
void EntityRenderer<StateType>::Changed()
{
}
