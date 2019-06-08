// Genetic Engine Class
//
// Author: Matthew Knight
// File Name: genetic-engine.hpp
// Date: 2019-06-07

#pragma once

#include <algorithm>
#include <exception>
#include <iterator>
#include <optional>
#include <set>
#include <tuple>
#include <vector>

namespace OpenGA {
    enum class StopReason { None, MaxGenerations, StallAverage, StallBest };

    struct Options {
        std::size_t population = 1000;
        std::size_t maxGeneration = 50;
        std::size_t eliteCount = 5;
        std::size_t truncateCount = 50;
    };

    /**
     * Class that encapsulates the GA
     */
    template <typename Chromozome>
    class Engine {
        const Options opts;
        double mutationRate = 0.001;

        std::size_t generationNum = 0;
        std::vector<Chromozome> generation;
        std::mt19937_64 mersenne;

        // TODO: add rate/improvement tracking
        // TODO: multithreading -- for GA itself

        StopReason checkExitConditions() {
            if (generationNum >= opts.maxGeneration)
                return StopReason::MaxGenerations;

            return StopReason::None;
        }

        StopReason iterateGeneration() {
            std::for_each(generation.begin(), generation.end(),
                          [](auto& chromozome) { chromozome.calculate(); });

            // sort them all based on fitness
            std::sort(generation.begin(), generation.end(),
                      [](auto const& lhs, auto const& rhs) {
                          return lhs.fitness > rhs.fitness;
                      });

            // check for exit condition
            auto stop = checkExitConditions();
            if (stop == StopReason::None) {
                // selection, crossover, mutation
                std::vector<Chromozome> nextGeneration;
                nextGeneration.reserve(opts.population);
                std::vector<double> scores;

                std::transform(
                    generation.begin(), generation.end(), scores.begin(),
                    std::back_inserter(scores),
                    [](auto& chromozome) { return chromozome.fitness; });

                std::discrete_distribution distribution(scores.begin(),
                                                        scores.end());

                auto backInserter = std::back_inserter(nextGenertation);
                std::generate_n(backInserter,
                                opts.population - opts.eliteCount -
                                    opts.truncateCount,
                                [&]() {
                                    auto mother = distribution(mersenne);
                                    decltype(first) father;
                                    do {
                                        father = distribution(mersenne);
                                    } while (mother == father);

                                    Chromozome child;
                                    do {
                                        child = generation[mother].crossover(
                                            generation[father]);
                                        child.mutate(mutationRate);
                                    } while (!child.isValid());

                                    return child;
                                });
                std::move(generation.begin(),
                          std::next(generation.begin(), opts.eliteCount),
                          backInserter);

                std::generate_n(backInserter, opts.truncateCount,
                                Chromozome::generate);
                generation = nextGeneration;
            }

            generationNum++;
            return stop;
        }

      public:
        Engine(Options const& opts)
            : opts(opts) {
            generation.reserve(opts.population);
            std::random_device device;
            mersenne.seed(device());
            std::generate_n(std::back_inserter(generation),
                            opts.population - generation.size(),
                            Chromozome::generate);
        }

        template <typename... Seeds>
        Engine(Options const& opts, Seeds... seeds)
            : Engine(opts) {
            static_assert(population < sizeof...(seeds));
            (generation.push_back(seeds), ...);
        }

        std::tuple<Chromozome, StopReason> solve() {
            StopReason stop = StopReason::None;
            while (stop == StopReason::None)
                stop = iterateGeneration();

            if (generation.empty())
                throw std::runtime_error("generation is empty");

            return std::make_tuple(generation.front(), stop);
        }
    };
} // namespace OpenGA
