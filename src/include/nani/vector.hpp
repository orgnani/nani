// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#ifndef NANI_VECTOR_HPP
#define NANI_VECTOR_HPP

#include <nani/meta.hpp>
#include <nani/smath.hpp>
#include <nani/static_array.hpp>
#include <nani/vector.fwd.hpp>
#include <nani/wrap_iterator.hpp>
#include <stdexcept>

namespace nani {
template <typename F, std::size_t N>
class vector {
public:
  using value_type = F;
  static constexpr std::size_t dim = N;

public:
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = value_type const&;

  using pointer = value_type*;
  using const_pointer = value_type const*;

  using iterator = nani::wrap_iterator<pointer>;
  using const_iterator = nani::wrap_iterator<const_pointer>;

  using difference_type = std::ptrdiff_t;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
  vector() = default;

  template <typename... U>
  constexpr explicit vector(U... u)
    requires(sizeof...(U) == N and std::conjunction_v<std::is_same<F, U>...>);

  constexpr explicit vector(value_type x) noexcept
    requires(N == 1);

  constexpr static auto create(nani::static_array<value_type, dim> const& static_array) noexcept
    -> vector;

  constexpr static auto fill(value_type value) noexcept -> vector;

  constexpr static auto unit(std::size_t i) noexcept -> vector;

public:
  constexpr explicit operator value_type() const noexcept
    requires(N == 1);

public:
  constexpr auto operator[](std::size_t i) const noexcept -> const_reference;

  constexpr auto operator[](std::size_t i) noexcept -> reference;

  constexpr auto begin() const noexcept -> const_iterator;

  constexpr auto end() const noexcept -> const_iterator;

  constexpr auto begin() noexcept -> iterator;

  constexpr auto end() noexcept -> iterator;

public:
  constexpr auto size() const noexcept -> size_type;

public:
  auto operator-=(vector const& other) noexcept -> vector&;
  auto operator+=(vector const& other) noexcept -> vector&;

private:
  nani::static_array<value_type, dim> x_{};
};

template <typename T, std::size_t N>
template <typename... U>

constexpr vector<T, N>::vector(U... u)
  requires(sizeof...(U) == N and std::conjunction_v<std::is_same<T, U>...>)
: x_{u...}
{
}

template <typename F, std::size_t N>

constexpr auto operator==(vector<F, N> const& a, vector<F, N> const& b) noexcept -> bool;

template <typename F, std::size_t N>

constexpr auto operator!=(vector<F, N> const& a, vector<F, N> const& b) noexcept -> bool;

template <typename F, std::size_t N>

constexpr auto operator+(vector<F, N> const& a, vector<F, N> const& b) noexcept -> vector<F, N>;

template <typename F, std::size_t N>

constexpr auto operator-(vector<F, N> const& a, vector<F, N> const& b) noexcept -> vector<F, N>;

template <typename F, std::size_t N>

constexpr auto operator*(vector<F, N> const& a, vector<F, N> const& b) noexcept -> F;

template <typename F, std::size_t N>

constexpr auto operator*(vector<F, N> const& a, F b) noexcept -> vector<F, N>;

template <typename F, std::size_t N>

constexpr auto operator*(F a, vector<F, N> const& b) noexcept -> vector<F, N>;

template <typename F, std::size_t N>

constexpr auto operator/(vector<F, N> const& a, F b) noexcept -> vector<F, N>;

template <typename F>

constexpr auto operator^(vector<F, 2> const& a, vector<F, 2> const& b) noexcept -> F;

template <typename F, std::size_t N>

constexpr auto normalize(vector<F, N> const& a) noexcept -> vector<F, N>;

template <typename F, std::size_t N>

constexpr auto l2_norm(vector<F, N> const& a) noexcept -> F;

template <typename F>

constexpr auto normal(vector<F, 2> const& a) noexcept -> vector<F, 2>;
//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------

template <typename F, std::size_t N>

constexpr vector<F, N>::vector(value_type x) noexcept
  requires(N == 1)
{
  x_[0] = x;
}

template <typename F, std::size_t N>
constexpr auto vector<F, N>::create(
  nani::static_array<value_type, dim> const& static_array) noexcept -> vector
{
  vector v{};
  v.x_ = static_array;
  return v;
}

template <typename F, std::size_t N>

constexpr auto vector<F, N>::fill(value_type value) noexcept -> vector
{
  vector v{};
  for (std::size_t i = 0; i < N; ++i) {
    v[i] = value;
  }
  return v;
}

template <typename F, std::size_t N>

constexpr auto vector<F, N>::unit(std::size_t index) noexcept -> vector
{
  auto v = vector::fill(0);
  if constexpr (meta::is_debug_build()) {
    if (index > N) {
      throw std::runtime_error("Out of bounds Access");
    }
  }
  v[index] = 1;
  return v;
}

template <typename F, std::size_t N>

constexpr vector<F, N>::operator value_type() const noexcept
  requires(N == 1)
{
  return x_[0];
}

template <typename F, std::size_t N>

constexpr auto vector<F, N>::operator[](std::size_t i) const noexcept -> value_type const&
{
  return x_[i];
}

template <typename F, std::size_t N>

constexpr auto vector<F, N>::begin() const noexcept -> const_iterator
{
  return x_.begin();
}

template <typename F, std::size_t N>

constexpr auto vector<F, N>::begin() noexcept -> iterator
{
  return x_.begin();
}

template <typename F, std::size_t N>

constexpr auto vector<F, N>::end() const noexcept -> const_iterator
{
  return x_.end();
}

template <typename F, std::size_t N>

constexpr auto vector<F, N>::end() noexcept -> iterator
{
  return x_.end();
}

template <typename F, std::size_t N>

constexpr auto vector<F, N>::operator[](std::size_t i) noexcept -> value_type&
{
  return x_[i];
}

template <typename F, std::size_t N>

constexpr auto vector<F, N>::size() const noexcept -> size_type
{
  return dim;
}

template <typename F, std::size_t N>

auto vector<F, N>::operator-=(vector const& other) noexcept -> vector&
{
  for (std::size_t i = 0; i < N; ++i) {
    x_[i] -= other[i];
  }
  return *this;
}

template <typename F, std::size_t N>

auto vector<F, N>::operator+=(vector const& other) noexcept -> vector&
{
  for (std::size_t i = 0; i < N; ++i) {
    x_[i] += other[i];
  }
  return *this;
}

template <typename F, std::size_t N>

constexpr auto operator==(vector<F, N> const& a, vector<F, N> const& b) noexcept -> bool
{
  for (std::size_t i = 0; i < N; ++i) {
    if (nani::abs(a[i] - b[i]) > 0) {
      return false;
    }
  }
  return true;
}

template <typename F, std::size_t N>

constexpr auto operator!=(vector<F, N> const& a, vector<F, N> const& b) noexcept -> bool
{
  return (not(a == b));
}

template <typename F, std::size_t N>

constexpr auto operator+(vector<F, N> const& a, vector<F, N> const& b) noexcept -> vector<F, N>
{
  using type = vector<F, N>;
  auto result = type();
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = a[i] + b[i];
  }
  return result;
}

template <typename F, std::size_t N>

constexpr auto operator-(vector<F, N> const& a, vector<F, N> const& b) noexcept -> vector<F, N>
{
  using type = vector<F, N>;
  auto result = type();
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = a[i] - b[i];
  }
  return result;
}

template <typename F, std::size_t N>

constexpr auto operator*(vector<F, N> const& a, vector<F, N> const& b) noexcept -> F
{
  F result = 0;
  for (std::size_t i = 0; i < N; ++i) {
    result += a[i] * b[i];
  }
  return result;
}

template <typename F, std::size_t N>

constexpr auto operator*(vector<F, N> const& a, F b) noexcept -> vector<F, N>
{
  using type = vector<F, N>;
  auto result = type();
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = a[i] * b;
  }
  return result;
}

template <typename F, std::size_t N>

constexpr auto operator*(F a, vector<F, N> const& b) noexcept -> vector<F, N>
{
  using type = vector<F, N>;
  auto result = type();
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = b[i] * a;
  }
  return result;
}

template <typename F, std::size_t N>

constexpr auto operator/(vector<F, N> const& a, F b) noexcept -> vector<F, N>
{
  using type = vector<F, N>;
  auto result = type();
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = a[i] / b;
  }
  return result;
}

template <typename F, std::size_t N>

constexpr auto l2_norm(vector<F, N> const& a) noexcept -> F
{
  F norm = 0.0;
  for (std::size_t i = 0; i < N; ++i) {
    norm += a[i] * a[i];
  }
  return nani::sqrt(norm);
}

template <typename F>

constexpr auto operator^(vector<F, 2> const& a, vector<F, 2> const& b) noexcept -> F
{
  return a[0] * b[1] - a[1] * b[0];
}

template <typename F, std::size_t N>

constexpr auto normalize(vector<F, N> const& a) noexcept -> vector<F, N>
{
  auto result = a;
  auto const norm = l2_norm(a);
  for (std::size_t i = 0; i < N; ++i) {
    result[i] /= norm;
  }
  return result;
}

template <typename F>

constexpr auto normal(vector<F, 2> const& a) noexcept -> vector<F, 2>
{
  return vector<F, 2>::create(nani::static_array<F, 2>(-a[1], a[0]));
}
} // namespace nani

#endif // NANI_VECTOR_HPP
