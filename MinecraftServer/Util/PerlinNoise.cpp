#include "PerlinNoise.h"

#include "EngineDefaults.h"

PerlinNoise::PerlinNoise(const int octave) : Octave(octave)
{
}

std::vector<int> PerlinNoise::Generate(const size_t width, const size_t height) const
{
    std::vector<int> result;
    result.reserve(width * height);
    std::vector<int> table;
    table.reserve(width * height);
    for (size_t i = 0; i < width * height; i++)
    {
        table.push_back(0);
        result.push_back(0);
    }
    for (size_t step = width >> Octave, y = 0; y < height; y += step)
    {
        for (size_t x = 0; x < width; x += step)
        {
            table[(x + y * width)] = EngineDefaults::GetNext<int>(-128, 128) * FUZZINESS;
        }
    }

    for (size_t step = width >> Octave; step > 1; step /= 2)
    {
        const size_t max = 256 * (step << Octave);
        const size_t halfStep = step >> 1;

        for (size_t y = 0; y < height; y += step)
        {
            for (size_t x = 0; x < width; x += step)
            {
                const int value = table[(x % width + y % height * width)];

                const int stepValueX = table[((x + step) % width + y % height * width)];
                const int stepValueY = table[(x % width + (y + step) % height * width)];
                const int stepValueXy = table[((x + step) % width + (y + step) % height * width)];

                const int mutatedValue = (value + stepValueY + stepValueX + stepValueXy) / 4 + EngineDefaults::GetNext<int>(-static_cast<int>(max), static_cast<int>(max));

                table[(x + halfStep + (y + halfStep) * width)] = mutatedValue;
            }
        }
        for (size_t y = 0; y < height; y += step)
        {
            for (size_t x = 0; x < width; x += step)
            {
                // Get value at index
                const int value = table[(x + y * width)];

                // Add step to create index for value
                const int stepValueX = table[((x + step) % width + y * width)];
                const int stepValueY = table[(x + (y + step) % width * width)];

                // Add step and half step to create index for value
                const int halfStepValueXPos = table[((x + halfStep & width - 1) + (y + halfStep - step & height - 1) * width)];
                const int halfStepValueYPos = table[((x + halfStep - step & width - 1) + (y + halfStep & height - 1) * width)];

                // Add half step to create index for value
                const int halfStepValue = table[((x + halfStep) % width + (y + halfStep) % height * width)];

                // Combine values for new value
                const int mutatedValueX = (value + stepValueX + halfStepValue + halfStepValueXPos) / 4 + EngineDefaults::GetNext<int>(-static_cast<int>(max), static_cast<int>(max));
                const int mutatedValueY = (value + stepValueY + halfStepValue + halfStepValueYPos) / 4 + EngineDefaults::GetNext<int>(-static_cast<int>(max), static_cast<int>(max));

                // Update values in table
                table[(x + halfStep + y * width)] = mutatedValueX;
                table[(x + (y + halfStep) * width)] = mutatedValueY;
            }
        }
    }
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            result[(x + y * width)] = table[x % width + y % height * width] / 512 + 128;
        }
    }
    return result;
}
