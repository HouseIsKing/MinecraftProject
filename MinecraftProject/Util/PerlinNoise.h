#pragma once
#include <vector>

class PerlinNoise
{
    const int Octave;
    static constexpr int FUZZINESS = 16;
public:
    explicit PerlinNoise(int octave);
    [[nodiscard]] std::vector<int> Generate(size_t width, size_t height) const;
};
