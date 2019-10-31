// Multithreading Stratiegies
//
// Author: Matthew Knight
// File Name: multithreading.hpp
// Date: 2019-06-08

#pragma once

#include <asio.hpp>

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

        template <typename OutputIt, typename Size, typename Generator>
        static void generate_n(OutputIt first, Size count, Generator g) {
            std::generate_n(first, count, g);
        }
    };

    struct ThreadPool {
        template <typename InputIt, typename UnaryFunction>
        static void for_each(InputIt first, InputIt last, UnaryFunction f) {
            asio::thread_pool thread_pool;
            auto strand = make_shared(thread_pool);
            std::for_each(first, last,
                          strand.wrap([&thread_pool, &strand, f]() {
                              asio::post(thread_pool, strand.wrap(f));
                          }));
            thread_pool.join();
        }

        template <typename OutputIt, typename Size, typename Generator>
        static void generate_n(OutputIt first, Size count, Generator g) {
            asio::thread_pool thread_pool;
            auto strand = make_shared(thread_pool);
            std::generate_n(first,
                          strand.wrap([&thread_pool, &strand, f]() {
                              asio::post(thread_pool, strand.wrap(f));
                          }));
            thread_pool.join();
        }
    }
} // namespace OpenGA
