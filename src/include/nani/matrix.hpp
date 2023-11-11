// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#ifndef NANI_MATRIX_HPP
#define NANI_MATRIX_HPP

#include <nani/matrix.fwd.hpp>
#include <nani/smath.hpp>
#include <nani/static_array.hpp>
#include <nani/vector.hpp>

namespace nani {
template <typename F, std::size_t R, std::size_t C>
class matrix {
private:
  static constexpr auto const nrow_ = R;
  static constexpr auto const ncol_ = C;

  using row_type = typename nani::static_array<double, ncol_>;

public:
  using size_type = nani::static_array<std::size_t, 2>;
  static constexpr auto dim = size_type(R, C);
  using storage_type = typename nani::static_array<row_type, nrow_>;

public:
  matrix() = default;

  constexpr explicit matrix(double s)
    requires(R == 1 and C == 1);

  template <std::size_t N>

  constexpr explicit matrix(vector<F, N> const& v)
    requires((C == 1 and N == R) or (R == 1 and N == C));

  static constexpr auto zero() noexcept -> matrix<F, R, C>;

  static constexpr auto identity() noexcept -> matrix<F, R, R>
    requires(R == C);

public:
  constexpr explicit operator double() const noexcept
    requires(R == 1 and C == 1);

  template <std::size_t N>

  constexpr explicit operator vector<F, N>() const noexcept
    requires((C == 1 and N == R) or (R == 1 and N == C));

public:
  constexpr auto operator[](std::size_t i) const noexcept -> row_type const&;

  constexpr auto operator[](std::size_t i) noexcept -> row_type&;

public:
  constexpr auto size() const noexcept -> nani::static_array<std::size_t, 2>;

private:
  storage_type x_{};
};

template <typename F, std::size_t R, std::size_t C>

constexpr auto operator+(matrix<F, R, C> const& a, matrix<F, R, C> const& b) -> matrix<F, R, C>;

template <typename F, std::size_t R, std::size_t C>

constexpr auto operator-(matrix<F, R, C> const& a, matrix<F, R, C> const& b) -> matrix<F, R, C>;

template <typename F, std::size_t R, std::size_t S, std::size_t T>

constexpr auto operator-(matrix<F, R, S> const& a, matrix<F, S, T> const& b) -> matrix<F, R, T>;

template <typename F, std::size_t R, std::size_t C>

constexpr auto operator*(matrix<F, R, C> const& a, vector<F, C> const& b) -> vector<F, R>;

template <typename F, std::size_t R, std::size_t C>

constexpr auto operator*(vector<F, R> const& a, matrix<F, R, C> const& b) -> vector<F, C>;

template <typename F, std::size_t R, std::size_t C>

constexpr auto operator*(matrix<F, R, C> const& a, double b) -> matrix<F, R, C>;

template <typename F, std::size_t R, std::size_t C>

constexpr auto operator*(double a, matrix<F, R, C> const& b) -> matrix<F, R, C>;

template <typename F, std::size_t R, std::size_t C>

constexpr auto operator/(matrix<F, R, C> const& a, double b) -> matrix<F, R, C>;

template <typename F, std::size_t R, std::size_t C>

constexpr auto outer(vector<F, R> const& a, vector<F, C> const& b) -> matrix<F, R, C>;

/* \brief Compute determinant of a Small Matrix
 *
 * For R between 0 and 5, n! < n^3. The biggest matrix that we will have will
 * be a 4x4 matrix for the calculation of characteristic variables. Even so,
 * to prevent misuse, we throw an error for R>5.
 *
 * PreConditions : R<6
 */
template <typename F, std::size_t R>
constexpr auto determinant(matrix<F, R, R> const& a) -> double
  requires(R < 6);

//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------

template <typename F, std::size_t R, std::size_t C>
constexpr matrix<F, R, C>::matrix(double s)
  requires(R == 1 and C == 1)
{
  x_[0][0] = s;
}

template <typename F, std::size_t R, std::size_t C>
template <std::size_t N>
constexpr matrix<F, R, C>::matrix(vector<F, N> const& v)
  requires((C == 1 and N == R) or (R == 1 and N == C))
{
  if constexpr (R == 1) {
    for (std::size_t i = 0; i < C; ++i) {
      x_[0][i] = v[i];
    }
  }
  else {
    for (std::size_t i = 0; i < R; ++i) {
      x_[i][0] = v[i];
    }
  }
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto matrix<F, R, C>::operator[](std::size_t i) const noexcept -> row_type const&
{
  return x_[i];
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto matrix<F, R, C>::operator[](std::size_t i) noexcept -> row_type&
{
  return x_[i];
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto matrix<F, R, C>::identity() noexcept -> matrix<F, R, R>
  requires(R == C)
{
  matrix result = zero();
  for (std::size_t i = 0; i < R; ++i) {
    result[i][i] = 1.0;
  }
  return result;
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto matrix<F, R, C>::zero() noexcept -> matrix<F, R, C>
{
  matrix result;
  for (std::size_t i = 0; i < R; ++i) {
    for (std::size_t j = 0; j < C; ++j) {
      result[i][j] = 0.0;
    }
  }
  return result;
}

template <typename F, std::size_t R, std::size_t C>
constexpr matrix<F, R, C>::operator double() const noexcept
  requires(R == 1 and C == 1)
{
  return x_[0][0];
}

template <typename F, std::size_t R, std::size_t C>
template <std::size_t N>
constexpr matrix<F, R, C>::operator vector<F, N>() const noexcept
  requires((C == 1 and N == R) or (R == 1 and N == C))
{
  if constexpr (C == 1) {
    auto x = vector<F, N>();
    for (std::size_t i = 0; i < R; ++i) {
      x[i] = x_[i][0];
      return x;
    }
  }
  else {
    return x_[0];
  }
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto matrix<F, R, C>::size() const noexcept -> nani::static_array<std::size_t, 2>
{
  return nani::static_array<std::size_t, 2>(R, C);
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto operator+(matrix<F, R, C> const& a, matrix<F, R, C> const& b) -> matrix<F, R, C>
{
  auto result = matrix<F, R, C>();
  for (std::size_t i = 0; i < R; ++i) {
    for (std::size_t j = 0; j < C; ++j) {
      result[i][j] = a[i][j] + b[i][j];
    }
  }
  return result;
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto operator-(matrix<F, R, C> const& a, matrix<F, R, C> const& b) -> matrix<F, R, C>
{
  auto result = matrix<F, R, C>();
  for (std::size_t i = 0; i < R; ++i) {
    for (std::size_t j = 0; j < C; ++j) {
      result[i][j] = a[i][j] - b[i][j];
    }
  }
  return result;
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto operator*(matrix<F, R, C> const& a, vector<F, C> const& b) -> vector<F, R>
{
  auto result = vector<F, R>();
  for (std::size_t i = 0; i < R; ++i) {
    result[i] = 0;
    for (std::size_t j = 0; j < C; ++j) {
      result[i] += a[i][j] * b[j];
    }
  }
  return result;
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto operator*(vector<F, R> const& a, matrix<F, R, C> const& b) -> vector<F, C>
{
  auto result = vector<F, C>();
  for (std::size_t i = 0; i < C; ++i) {
    result[i] = 0;
    for (std::size_t j = 0; j < R; ++j) {
      result[i] += a[j] * b[j][i];
    }
  }
  return result;
}

template <typename F, std::size_t R, std::size_t S, std::size_t T>
constexpr auto operator-(matrix<F, R, S> const& a, matrix<F, S, T> const& b) -> matrix<F, R, T>
{
  auto result = matrix<F, S, T>();
  for (std::size_t i = 0; i < R; ++i) {
    for (std::size_t j = 0; j < T; ++j) {
      result[i][j] = 0;
      for (std::size_t k = 0; k < S; ++k) {
        result[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto operator*(matrix<F, R, C> const& a, double b) -> matrix<F, R, C>
{
  auto result = a;
  for (std::size_t i = 0; i < R; ++i) {
    for (std::size_t j = 0; j < C; ++j) {
      result[i][j] *= b;
    }
  }
  return result;
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto operator*(double a, matrix<F, R, C> const& b) -> matrix<F, R, C>
{
  auto result = b;
  for (std::size_t i = 0; i < R; ++i) {
    for (std::size_t j = 0; j < C; ++j) {
      result[i][j] *= a;
    }
  }
  return result;
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto operator/(matrix<F, R, C> const& a, double b) -> matrix<F, R, C>
{
  auto result = a;
  for (std::size_t i = 0; i < R; ++i) {
    for (std::size_t j = 0; j < C; ++j) {
      result[i][j] /= b;
    }
  }
  return result;
}

template <typename F, std::size_t R, std::size_t C>
constexpr auto outer(vector<F, R> const& a, vector<F, C> const& b) -> matrix<F, R, C>
{
  auto result = matrix<F, R, C>();
  for (std::size_t i = 0; i < R; ++i) {
    for (std::size_t j = 0; j < C; ++j) {
      result[i][j] = a[i] * b[j];
    }
  }
  return result;
}

template <typename F, std::size_t R>
constexpr auto determinant(matrix<F, R, R> const& a) -> double
  requires(R < 6)
{
  nani::static_array<std::size_t, R> permutation{};
  for (std::size_t i = 0; i < R; ++i) {
    permutation[i] = i;
  }

  constexpr auto const npermutation = factorial(R);

  double determinant = 0;
  for (std::size_t i = 0; i < npermutation; ++i) {
    double term = 1.0;
    for (std::size_t j = 0; j < R; ++j) {
      term *= a[j][permutation[j]];
    }
    determinant += term;
    std::next_permutation(permutation.begin(), permutation.end());
  }
  return determinant;
}

} // namespace nani
#endif // NANI_MATRIX_HPP
