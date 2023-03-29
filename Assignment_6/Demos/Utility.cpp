#include "Utility.h"
#include <cstdlib>
#include <cstdint>
#include <array>
#include <random>
#include <climits>

/* Random number seed to use when asking for a consistent hash function. */
const int kStableSeed = 137;

namespace {
    using TabulationTable = std::array<std::array<std::uint32_t, 256>, 4>;

    TabulationTable tabulationTable(std::uint32_t seed) {
        std::mt19937 engine(seed);
        std::uniform_int_distribution<std::uint32_t> dist;

        TabulationTable table;
        for (std::size_t i = 0; i < 4; i++) {
            for (std::size_t byte = 0; byte < 256; byte++) {
                table[i][byte] = dist(engine);
            }
        }
        return table;
    }

    std::function<int(int)> tabulationHashFunction(int seed) {
        auto table = tabulationTable(std::uint32_t(seed));
        return [table] (std::uint32_t key) {
            std::uint32_t result = 0;
            for (size_t i = 0; i < 4; i++) {
                result ^= table[i][(key >> (i * 8)) & 0xFF];
            }
            return result;
        };
    }
}

HashFunction<std::string> Hash::random(int numSlots) {
    auto scrambler = tabulationHashFunction(randomInteger(0, 0x7FFFFFFF));
    return HashFunction<std::string>::wrap(numSlots,
        [=] (const std::string& str) {
            return scrambler(hashCode(str));
        });
}

HashFunction<std::string> Hash::consistentRandom(int numSlots) {
    return forSize(numSlots);
}

HashFunction<std::string> Hash::zero(int numSlots) {
    return constant(numSlots, 0);
}

HashFunction<std::string> Hash::constant(int numSlots, int value) {
    return HashFunction<std::string>::wrap(numSlots, [value](const std::string&) { return value; });
}

HashFunction<std::string> Hash::identity(int numSlots) {
    return HashFunction<std::string>::wrap(numSlots, [](const std::string& str) { return strtol(str.c_str(), nullptr, 10); });
}
