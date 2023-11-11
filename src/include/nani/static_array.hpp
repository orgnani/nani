// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#ifndef NANI_STATIC_ARRAY_HPP
#define NANI_STATIC_ARRAY_HPP

#include <nani/wrap_iterator.hpp>

namespace nani {
template <typename T, std::size_t N>
class static_array {

public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = value_type const&;

  using pointer = value_type*;
  using const_pointer = value_type const*;

  using iterator = wrap_iterator<pointer>;
  using const_iterator = wrap_iterator<const_pointer>;

  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
  static_array() = default;

  template <typename... U>

  constexpr explicit static_array(U... u)
    requires(sizeof...(U) == N and std::conjunction_v<std::is_same<T, U>...>);

public:
  constexpr auto begin() const noexcept -> const_iterator;

  constexpr auto end() const noexcept -> const_iterator;

  constexpr auto begin() noexcept -> iterator;

  constexpr auto end() noexcept -> iterator;

  constexpr auto operator[](size_type index) const noexcept -> const_reference;

  constexpr auto operator[](size_type index) noexcept -> reference;

  constexpr auto operator=(value_type const& v) noexcept -> static_array&;

  constexpr auto size() const noexcept -> size_type;

private:
  // NOLINTNEXTLINE(modernize-avoid-c-arrays)
  value_type value_[N];
};

template <class T, class... U>
static_array(T, U...) -> static_array<T, sizeof...(U)>;

template <typename T, typename... Args>
constexpr auto to_static_array(Args&&... args) -> nani::static_array<T, sizeof...(Args)>
{
  return {{std::forward<Args>(args)...}};
}

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

template <typename T, std::size_t N>
template <typename... U>

constexpr static_array<T, N>::static_array(U... u)
  requires(sizeof...(U) == N and std::conjunction_v<std::is_same<T, U>...>)
: value_{u...}
{
}

template <typename T, std::size_t N>

constexpr auto static_array<T, N>::begin() const noexcept -> const_iterator
{
  return wrap_iterator<const_pointer>(&value_[0]);
}

template <typename T, std::size_t N>

constexpr auto static_array<T, N>::end() const noexcept -> const_iterator
{
  return wrap_iterator<const_pointer>(&value_[0] + N);
}

template <typename T, std::size_t N>

constexpr auto static_array<T, N>::begin() noexcept -> iterator
{
  return wrap_iterator<pointer>(&value_[0]);
}

template <typename T, std::size_t N>

constexpr auto static_array<T, N>::end() noexcept -> iterator
{
  return wrap_iterator<pointer>(&value_[0] + N);
}

template <typename T, std::size_t N>

constexpr auto static_array<T, N>::operator[](size_type index) const noexcept -> const_reference
{
  return value_[index];
}

template <typename T, std::size_t N>

constexpr auto static_array<T, N>::operator[](size_type index) noexcept -> reference
{
  return value_[index];
}

template <typename T, std::size_t N>

constexpr auto static_array<T, N>::operator=(value_type const& v) noexcept -> static_array&
{
  for (std::size_t i = 0; i < N; ++i) {
    value_[i] = v;
  }
  return *this;
}

template <typename T, std::size_t N>

constexpr auto static_array<T, N>::size() const noexcept -> size_type
{
  return N;
}

} // namespace nani

#endif // NANI_STATIC_ARRAY_HPP
