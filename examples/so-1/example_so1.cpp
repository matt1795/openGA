// This library is free and distributed under
// Mozilla Public License Version 2.0.

#include "openga/openga.hpp"
#include <iostream>

struct MySolution {
    double x;
    double y;

    // required interface
    std::optional<double> cost;
    static MySolution generate() { return {}; }
    void calculate() {
        if (!cost)
            cost = 1.0;
    }
};

int main() {
    OpenGA::StopWatch<std::chrono::steady_clock> timer;

    OpenGA::Engine<MySolution> engine(20, 1000, 10);
    auto [solution, reason] = engine.solve();

    std::cout << "took " << timer.getDuration().count() << "milliseconds"
              << std::endl;
    return 0;
}
