// Counting ones in an int
//
// Author: Matthew Knight
// File Name: counting-ones.cpp
// Date: 2019-06-09

#include "openga/openga.hpp"

#include <iostream>
#include <random>

std::mt19937 mersenne;

struct IntSolution {
    std::uint32_t x;
    static const auto digits = std::numeric_limits<decltype(x)>::digits;
    inline static std::uniform_int_distribution intDist{};
    inline static std::uniform_int_distribution crossDist{0, digits};

    IntSolution crossover(IntSolution const& other) const {
        auto point = crossDist(mersenne);
        constexpr auto max = std::numeric_limits<decltype(x)>::max();
        return {(x & (max << point)) | (other.x & (max >> (digits - point)))};
    }

    void mutate(double mutationRate) {
        std::bernoulli_distribution dist(mutationRate);

        for (auto i = 0; i < digits; i++)
            if (dist(mersenne))
                x ^= 1 << i;
    }

    bool isValid() const { return true; }

    double fitness() {
        double ret = 0.0;
        for (auto i = 0; i < digits; i++)
            if (x & (1 << i))
                ret += 1.0;

        return ret;
    }

    static IntSolution generate() {
        return {static_cast<std::uint32_t>(intDist(mersenne))};
    }
};

int main() {
    OpenGA::StopWatch<std::chrono::steady_clock> timer;
    OpenGA::Engine<IntSolution> engine;
    auto [solution, reason] = engine.solve();

    std::cout << "took " << timer.getDuration().count() << "ms" << std::endl;
    std::cout << "solution: " << std::hex << solution.x << std::endl;
    return 0;
}
