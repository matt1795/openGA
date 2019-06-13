// Multithreading Stratiegies
//
// Author: Matthew Knight
// File Name: multithreading.hpp
// Date: 2019-06-08

#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace OpenGA {
    /**
     * Static class for using a single thread in portions of the GA where
     * multithreading is supported
     */
    struct SingleThread {
        template <typename InputIt, typename UnaryFunction>
        static void for_each(InputIt first, InputIt last, UnaryFunction f) {
            std::for_each(first, last, f);
        }

        template <typename OutputId, typename Size, typename Generator>
        static void generate_n(OutputId first, Size count, Generator g) {
            std::generate_n(first, count, g);
        }
    };
} // namespace OpenGA
