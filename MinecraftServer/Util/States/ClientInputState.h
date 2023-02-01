#pragma once
#include <cstdint>

struct ClientInputState
{
    float MouseX{0.0F};
    float MouseY{0.0F};
    bool LeftMouseButtonPressed{false};
    bool RightMouseButtonPressed{false};
    int8_t ForwardAxis{0};
    int8_t RightAxis{0};
    bool JumpPressed{false};
    bool ResetPressed{false};
    bool SpawnZombiePressed{false};
    bool OnePressed{false};
    bool TwoPressed{false};
    bool ThreePressed{false};
    bool FourPressed{false};
    bool FivePressed{false};
    std::vector<uint8_t> operator-(const ClientInputState& inputState) const;
};

inline std::vector<uint8_t> ClientInputState::operator-(const ClientInputState& inputState) const
{
    std::vector<uint8_t> result;
    result.push_back(0);
    if (abs(MouseX - inputState.MouseX) > 0.001F)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::MouseX;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.MouseX), sizeof MouseX);
    }
    if (abs(MouseY - inputState.MouseY) > 0.001F)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::MouseY;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.MouseY), sizeof MouseY);
    }
    if (LeftMouseButtonPressed != inputState.LeftMouseButtonPressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::LeftMouseButton;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.LeftMouseButtonPressed), sizeof LeftMouseButtonPressed);
    }
    if (RightMouseButtonPressed != inputState.RightMouseButtonPressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::RightMouseButton;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.RightMouseButtonPressed), sizeof RightMouseButtonPressed);
    }
    if (ForwardAxis != inputState.ForwardAxis)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::ForwardAxis;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.ForwardAxis), sizeof ForwardAxis);
    }
    if (RightAxis != inputState.RightAxis)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::RightAxis;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.RightAxis), sizeof RightAxis);
    }
    if (JumpPressed != inputState.JumpPressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::Jump;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.JumpPressed), sizeof JumpPressed);
    }
    if (ResetPressed != inputState.ResetPressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::ResetPos;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.ResetPressed), sizeof ResetPressed);
    }
    if (SpawnZombiePressed != inputState.SpawnZombiePressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::SpawnZombie;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.SpawnZombiePressed), sizeof SpawnZombiePressed);
    }
    if (OnePressed != inputState.OnePressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::OnePressed;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.OnePressed), sizeof OnePressed);
    }
    if (TwoPressed != inputState.TwoPressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::TwoPressed;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.TwoPressed), sizeof TwoPressed);
    }
    if (ThreePressed != inputState.ThreePressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::ThreePressed;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.ThreePressed), sizeof ThreePressed);
    }
    if (FourPressed != inputState.FourPressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::FourPressed;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.FourPressed), sizeof FourPressed);
    }
    if (FivePressed != inputState.FivePressed)
    {
        result[0]++;
        constexpr auto temp = EChangeTypePlayer::FivePressed;
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&temp), sizeof(EChangeTypePlayer));
        EngineDefaults::EmplaceDataInVector(result, reinterpret_cast<const uint8_t*>(&inputState.FivePressed), sizeof FivePressed);
    }
    return result;
}
