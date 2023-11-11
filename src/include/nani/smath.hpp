// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#ifndef NANI_SMATH_HPP
#define NANI_SMATH_HPP

#include <cmath>
#include <limits>
#include <type_traits>

namespace nani {
template <typename T>
constexpr auto abs(T const& a) -> T
{
  if (a < 0) {
    return -a;
  }
  return a;
}

constexpr auto factorial(std::size_t n) -> std::size_t
{
  if (n == 0) {
    return 1;
  }
  return n * factorial(n - 1);
}

template <typename T>
constexpr auto pow(T const& a, std::size_t n) -> T
{
  T result = 1.0;
  while (n != 0) {
    result *= a;
    --n;
  }
  return result;
}

template <typename T>
constexpr auto sqrt(T const& a) -> T
{
  if (std::is_constant_evaluated()) {
    T result = 1;
    constexpr auto tol = std::numeric_limits<T>::epsilon();
    while (abs(a - result * result) > tol * a) {
      result = (result + a / result) / 2.0;
    }
    return result;
  }

  return std::sqrt(a);
}

template <typename T>
constexpr auto max(T const& a, T const& b) -> T
{
  if (a > b) {
    return a;
  }
  return b;
}
} // namespace nani

#endif // NANI_SMATH_HPP
