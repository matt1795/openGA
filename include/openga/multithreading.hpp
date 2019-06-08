// Multithreading Stratiegies
//
// Author: Matthew Knight
// File Name: multithreading.hpp
// Date: 2019-06-08

#pragma once

#include <algorithm>
#include <thread>

namespace OpenGA {
    struct SingleThread {
        template <typename InputIt, typename UnaryFunction>
        static UnaryFunction for_each(InputIt first, InputIt last,
                                      UnaryFunction f) {
            return std::for_each(first, last, f);
        }

        template <typename OutputId, typename Size, typename Generator>
        static void generate_n(OutputId first, Size count, Generator g) {
            std::generate_n(first, count, g);
        }
    };

    // TODO: create a thread pool implementation
    template <auto num>
    struct ThreadPool {
        template <typename InputIt, typename UnaryFunction>
        static UnaryFunction for_each(InputIt first, InputIt last,
                                      UnaryFunction f);
        template <typename OutputId, typename Size, typename Generator>
        static void generate_n(OutputId first, Size count, Generator g);
    };

    // TODO: create a thread branching implementation
    template <auto max>
    struct ThreadBranching {
        template <typename InputIt, typename UnaryFunction>
        static UnaryFunction for_each(InputIt first, InputIt last,
                                      UnaryFunction f);

        template <typename OutputId, typename Size, typename Generator>
        static void generate_n(OutputId first, Size count, Generator g);
    };
} // namespace OpenGA
