// This library is free and distributed under
// Mozilla Public License Version 2.0.

#include "openga/openga.hpp"
#include <iostream>

struct MySolution {
    double x;
    double y;

    // required interface
    MySolution crossover(MySolution const& other) const {
        MySolution sol(other);
        return sol;
    }

    void mutate(double mutationRate) {}
    bool isValid() const { return true;}
    double fitness() { return 1.0; }

    static MySolution generate() { return {}; }

};

int main() {
    OpenGA::StopWatch<std::chrono::steady_clock> timer;
    OpenGA::Engine<MySolution> engine;
    auto [solution, reason] = engine.solve();

    std::cout << "took " << timer.getDuration().count() << "ms"
              << std::endl;
    return 0;
}
