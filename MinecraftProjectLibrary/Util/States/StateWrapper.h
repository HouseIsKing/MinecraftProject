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
    virtual void WriteChangesToVector(std::vector<uint8_t>& changes, const ChangesType& changeType) const = 0;
    virtual void ClearAllChanges(const ChangesType& change) = 0;

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
    virtual void WriteChangesToVector(std::vector<uint8_t>& changes) const;
    virtual void ClearAllChanges();
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

template <typename StateType, typename ChangesType> requires std::is_base_of_v<State, StateType> && (std::is_integral_v<
    ChangesType> || std::is_enum_v<ChangesType>)
void StateWrapper<StateType, ChangesType>::WriteChangesToVector(std::vector<uint8_t>& changes) const
{
    const auto changesCount = static_cast<uint16_t>(Changes.size());
    EngineDefaults::EmplaceReplaceDataInVector(changes, &changesCount);
    for (const ChangesType& change : Changes)
    {
        EngineDefaults::EmplaceReplaceDataInVector(changes, &change);
        WriteChangesToVector(changes, change);
    }
}

template <typename StateType, typename ChangesType> requires std::is_base_of_v<State, StateType> && (std::is_integral_v<
    ChangesType> || std::is_enum_v<ChangesType>)
void StateWrapper<StateType, ChangesType>::ClearAllChanges()
{
    for (const ChangesType& change : Changes)
    {
        ClearAllChanges(change);
    }
    Changes.clear();
}
