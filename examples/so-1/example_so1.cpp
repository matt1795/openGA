// Counting ones in an int
//
// Author: Matthew Knight
// File Name: counting-ones.cpp
// Date: 2019-06-09

#include "openga/openga.hpp"

#include <iostream>
#include <cstdlib>

struct IntSolution {
    int x;

    // required interface
    IntSolution crossover(IntSolution const& other) const {
        // TODO: int crossover, choose a random crossover point, randomly select
        // child
        return other;
    }

    void mutate(double mutationRate) {
        // int mutation -- each bit has <mutationRate> chance of flipping
    }

    // All ints are valid
    bool isValid() const { return true;}

    double fitness() { 
        // iterate through every bit and count the number of ones
        return 1.0; }

    static IntSolution generate() { return {rand()}; }
};

int main() {
    srand(0);
    OpenGA::StopWatch<std::chrono::steady_clock> timer;
    OpenGA::Engine<IntSolution> engine;
    auto [solution, reason] = engine.solve();

    std::cout << "took " << timer.getDuration().count() << "ms"
              << std::endl;
    return 0;
}
