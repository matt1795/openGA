// Genetic Engine Class
// double fitness { return 1.0; }
//
// Author: Matthew Knight
// File Name: genetic-engine.hpp
// Date: 2019-06-07

#pragma once

#include "multithreading.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <optional>
#include <random>
#include <set>
#include <tuple>
#include <vector>

namespace OpenGA {
    namespace {
        const auto validationError =
            std::runtime_error("Solution failed validation");
    }

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
    template <typename Solution, typename ThreadStrategy = SingleThread>
    class Engine {
        // wrapper class to simplify use
        struct Chromozome {
            Solution solution;
            std::optional<double> score;

            void calculate() {
                if (!score)
                    score = solution.fitness();
            }

            Chromozome crossover(Chromozome const& other) const {
                return Chromozome{solution.crossover(other.solution)};
            }

            void mutate(double mutateRate) { solution.mutate(mutateRate); }

            bool isValid() const { return solution.isValid(); }

            static Chromozome generate() {
                auto ret = Chromozome{Solution::generate()};
                if (!ret.solution.isValid())
                    throw validationError;

                return ret;
            }
        };

        const Options opts;
        double mutationRate = 0.001;

        std::size_t generationNum = 0;
        std::vector<Chromozome> generation;
        std::mt19937_64 mersenne;

        // TODO: add rate/improvement tracking
        // TODO: multithreading -- for GA itself

        StopReason checkExitConditions() {
            if (!(generationNum < opts.maxGeneration - 1))
                return StopReason::MaxGenerations;

            return StopReason::None;
        }

        StopReason iterateGeneration() {
            ThreadStrategy::for_each(
                generation.begin(), generation.end(),
                [](auto& chromozome) { chromozome.calculate(); });

            // validate fitnesses
            if (std::any_of(generation.begin(), generation.end(),
                            [](auto& chromozome) {
                                return !static_cast<bool>(chromozome.score);
                            })) {
                throw std::runtime_error("fitness was not calculated");
            }

            // sort them all based on fitness
            std::sort(generation.begin(), generation.end(),
                      [](auto const& lhs, auto const& rhs) {
                          return lhs.score.value() > rhs.score.value();
                      });

            // check for exit condition
            auto stop = checkExitConditions();
            if (stop == StopReason::None) {
                // selection, crossover, mutation
                std::vector<Chromozome> nextGeneration;
                nextGeneration.reserve(opts.population);
                std::vector<double> scores;

                std::transform(
                    generation.begin(), generation.end(),
                    std::back_inserter(scores),
                    [](auto& chromozome) { return chromozome.score.value(); });

                std::discrete_distribution dist(scores.begin(), scores.end());
                auto backInserter = std::back_inserter(nextGeneration);
                ThreadStrategy::generate_n(
                    backInserter,
                    opts.population - opts.eliteCount - opts.truncateCount,
                    [&]() {
                        auto mother = dist(mersenne);
                        decltype(mother) father;
                        do {
                            father = dist(mersenne);
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

                ThreadStrategy::generate_n(backInserter, opts.truncateCount,
                                           Chromozome::generate);
                generation = nextGeneration;
                generationNum++;
            }

            return stop;
        }

      public:
        Engine()
            : Engine(Options{}) {}

        Engine(Options const& opts)
            : opts(opts) {
            generation.reserve(opts.population);
            std::random_device device;
            mersenne.seed(device());
            ThreadStrategy::generate_n(std::back_inserter(generation),
                                       opts.population - generation.size(),
                                       Chromozome::generate);
        }

        template <typename... Seeds>
        Engine(Options const& opts, Seeds... seeds)
            : Engine(opts) {
            static_assert(opts.population < sizeof...(seeds));
            (generation.push_back(seeds), ...);
        }

        std::tuple<Solution, StopReason> solve() {
            StopReason stop = StopReason::None;
            while (stop == StopReason::None) {
                std::cout << "generation: " << generationNum << std::endl;
                stop = iterateGeneration();
            }

            if (generation.empty())
                throw std::runtime_error("generation is empty");

            return std::make_tuple(generation.front().solution, stop);
        }
    };
} // namespace OpenGA
