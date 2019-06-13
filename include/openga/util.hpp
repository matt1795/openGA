// Utility Classes
//
// Author: Matthew Knight
// File Name: util.hpp
// Date: 2019-06-10

#pragma once

#include <random>
#include <type_traits>

namespace OpenGA {
    struct NoRequirements {
        bool meetsRequirements() const { return false; }
    };

    struct AlwaysValid {
        bool isValid() const { return true; }
    }

    /**
     * Basic mutation function for integer types
     */
    template <typename T, typename Generator,
              typename = std::enable_if_t<std::is_integral_v<T>>>
    void mutate(double rate, Generator& g, T& val) {
        static constexpr auto digits = std::numeric_limits<T>::digits;
        std::bernoulli_distribution dist(rate);

        for (auto i = 0; i < digits; i++)
            if (dist(g))
                val ^= 1 << i;
    }

    /**
     * Basic crossover function for integer types
     */
    template <typename T, typename Generator,
              typename = std::enable_if_t<std::is_integral_v<T>>>
    T crossover(T const& lhs, T const& rhs, Generator& g) {
        static constexpr auto digits = std::numeric_limits<T>::digits;
        static constexpr auto max = std::numeric_limits<T>::max();
        static std::uniform_int_distribution dist{0, digits};

        auto point = dist(g);
        return {(lhs & (max << point)) | (rhs & (max >> (digits - point)))};
    }
} // namespace OpenGA
