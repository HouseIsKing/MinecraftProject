#pragma once
#include "Util/EngineDefaults.h"
#include <set>

template <typename StateType, typename ChangesType> requires std::is_base_of_v<State, StateType> and (std::is_integral_v<ChangesType> or std::is_enum_v<ChangesType>)
class StateWrapper
{
protected:
    std::set<ChangesType> Changes;
    StateType State;
    StateType OldState;

public:
    StateWrapper() = default;
    StateWrapper(const StateWrapper&) = delete;
    StateWrapper(StateWrapper&&) = delete;
    StateWrapper& operator=(const StateWrapper&) = delete;
    StateWrapper& operator=(StateWrapper&&) = delete;
    virtual ~StateWrapper() = default;
    explicit StateWrapper(const StateType& otherState);
    [[nodiscard]] const StateType& GetState() const;
    [[nodiscard]] const StateType& GetOldState() const;
    virtual void WriteChangesToVector(std::vector<uint8_t>& changes) const = 0;
    virtual void ClearAllChanges() = 0;
};

template <typename StateType, typename ChangesType> requires std::is_base_of_v<State, StateType> && (std::is_integral_v<
    ChangesType> || std::is_enum_v<ChangesType>)
StateWrapper<StateType, ChangesType>::StateWrapper(const StateType& otherState) : State(otherState), OldState(otherState)
{
}

template <typename StateType, typename ChangesType> requires std::is_base_of_v<State, StateType> && (std::is_integral_v<
    ChangesType> || std::is_enum_v<ChangesType>)
const StateType& StateWrapper<StateType, ChangesType>::GetState() const
{
    return State;
}

template <typename StateType, typename ChangesType> requires std::is_base_of_v<State, StateType> && (std::is_integral_v<
    ChangesType> || std::is_enum_v<ChangesType>)
const StateType& StateWrapper<StateType, ChangesType>::GetOldState() const
{
    return OldState;
}
