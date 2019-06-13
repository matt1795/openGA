// Counting ones in an int
//
// Author: Matthew Knight
// File Name: counting-ones.cpp
// Date: 2019-06-09

#include "openga/openga.hpp"

#include <iostream>
#include <random>

std::mt19937 mersenne;

// Solution providing the needed interface:
struct IntSolution {
    std::uint32_t x;

    static const auto digits = std::numeric_limits<std::uint32_t>::digits;
    inline static std::uniform_int_distribution intDist{};

    // The library comes with a generic crossover operator for integral types
    IntSolution crossover(IntSolution const& other) const {
        return {OpenGA::crossover(x, other.x, mersenne)};
    }

    // The mutationRate is the chance of any bit flipping
    void mutate(double mutationRate) {
        OpenGA::mutate(mutationRate, mersenne, x);
    }

    // All values are valid
    bool isValid() const { return true; }

    // The fitness is equal to the number of ones
    double fitness() {
        double ret = 0.0;
        for (auto i = 0; i < digits; i++)
            if (x & (1 << i))
                ret += 1.0;

        return ret;
    }

    // We know the answer we'll end with, so we can stop when we get there.
    bool meetsRequirements() const {
        return x == std::numeric_limits<std::uint32_t>::max();
    }

    // Generate any uint32_t
    static IntSolution generate() {
        return {static_cast<std::uint32_t>(intDist(mersenne))};
    }
};

int main() {
    OpenGA::StopWatch<std::chrono::steady_clock> timer;
    IntSolution seed{32};
    OpenGA::Engine<IntSolution, OpenGA::ThreadPool<4>> engine(OpenGA::Options{},
                                                              seed);
    auto [solution, reason] = engine.solve();

    std::cout << "took " << timer.getDuration().count() << "ms" << std::endl;
    std::cout << "solution: " << std::hex << solution.x << std::endl;
    return 0;
}
