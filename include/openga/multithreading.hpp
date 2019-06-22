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
#include <numeric>
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

        template <typename OutputIt, typename Size, typename Generator>
        static void generate_n(OutputIt first, Size count, Generator g) {
            std::generate_n(first, count, g);
        }
    };

    template <auto num>
    struct ThreadPool {
        template <typename InputIt, typename UnaryFunction>
        static void for_each(InputIt first, InputIt last, UnaryFunction f) {
            InputIt it = first;
            std::mutex mtx;
            std::vector<std::thread> threads(num);
            for (auto& th : threads) {
				th = std::thread([&]() {
					InputIt local;
					mtx.lock();
					while (it != last) {
						local = it;
						++it;
						mtx.unlock();
						f(*local);
						mtx.lock();
					}
				});
			}

            for (auto& th : threads)
                th.join();
        }

        template <typename OutputIt, typename Size, typename Generator>
        static void generate_n(OutputIt first, Size count, Generator g) {
            std::generate_n(first, count, g);
        }
    };
} // namespace OpenGA
