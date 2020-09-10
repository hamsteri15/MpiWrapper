#pragma once

#include <array>

namespace MpiWrapper::Utils {

// TODO Check if there's a better way
template <size_t N> constexpr std::array<int, N> cast_to_intarray(const std::array<size_t, N> arr) {

    std::array<int, N> ret{};
    std::copy(arr.begin(), arr.end(), ret.begin());
    return ret;
}

// TODO Check if there's a better way
template <size_t N>
constexpr std::array<size_t, N> cast_to_uintarray(const std::array<int, N> arr) {

    std::array<size_t, N> ret{};
    std::copy(arr.begin(), arr.end(), ret.begin());
    return ret;
}

// TODO Check if there's a better way
template <size_t N>
constexpr std::array<unsigned long long, N> cast_to_ulonglongarray(const std::array<size_t, N> arr) {

    std::array<unsigned long long, N> ret{};
    std::copy(arr.begin(), arr.end(), ret.begin());
    return ret;
}






} // namespace Utils