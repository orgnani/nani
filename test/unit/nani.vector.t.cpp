// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#include <catch2/catch_test_macros.hpp>
#include <nani/smath.hpp>
#include <nani/static_array.hpp>
#include <nani/vector.hpp>
#include <testmol/compat/catch_main.hpp>

TESTMOL_CATCH_MAIN("test/unit/cpp/bati/vector")

TEST_CASE("Constructor of vector<double,1> from Double", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto vector = nani::vector<double, 1>(4.3);
  static_assert(nani::abs(vector[0] - 4.3) < tol);
}

TEST_CASE("Constructor of vector<N> from static_array", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto static_array = nani::static_array<double, 2>{3.3, 3.5};
  constexpr auto vector = nani::vector<double, 2>::create(static_array);
  static_assert(vector.size() == 2);
  static_assert(nani::abs(vector[0] - 3.3) < tol);
  static_assert(nani::abs(vector[1] - 3.5) < tol);
}

TEST_CASE("Fill Constructor", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();

  SECTION("0")
  {
    constexpr auto vector = nani::vector<double, 2>::fill(3.4);
    static_assert(nani::abs(vector[0] - 3.4) < tol);
    static_assert(nani::abs(vector[0] - 3.4) < tol);
  }
}

TEST_CASE("Constructor of directional vectors", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();

  SECTION("0")
  {
    constexpr auto vector = nani::vector<double, 2>::unit(0);
    static_assert(nani::abs(vector[0] - 1) < tol);
    static_assert(nani::abs(vector[1] - 0) < tol);
  }

  SECTION("1")
  {
    constexpr auto vector = nani::vector<double, 2>::unit(1);
    static_assert(nani::abs(vector[0] - 0) < tol);
    static_assert(nani::abs(vector[1] - 1) < tol);
  }
}

TEST_CASE("Implicit Conversion to Double", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();

  constexpr auto v = nani::vector<double, 1>::fill(2.3);
  constexpr auto a = static_cast<double>(v);
  static_assert(nani::abs(a - 2.3) < tol * a);
}

TEST_CASE("Comparison Operators", "[all]")
{
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(3.0, 4.0));

  constexpr auto b = nani::vector<double, 2>::create(nani::static_array<double, 2>(3.0, 4.0));

  constexpr auto c = nani::vector<double, 2>::create(nani::static_array<double, 2>(4.0, 4.0));

  constexpr auto d = nani::vector<double, 2>::create(nani::static_array<double, 2>(3.0, 3.0));

  constexpr auto e = nani::vector<double, 2>::create(nani::static_array<double, 2>(5.0, 5.0));

  static_assert(a == b);
  static_assert(not(a != b));

  static_assert(a != c);
  static_assert(not(a == c));

  static_assert(a != d);
  static_assert(not(a == d));

  static_assert(a != e);
  static_assert(not(a == e));
}

TEST_CASE("operator +, 1D", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 1>::create(nani::static_array<double, 1>(5.0));
  constexpr auto b = nani::vector<double, 1>::create(nani::static_array<double, 1>(3.9));

  constexpr auto c = a + b;
  static_assert(nani::abs(c[0] - 8.9) < tol * a[0]);
}

TEST_CASE("operator +", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(8.3, 5.0));
  constexpr auto b = nani::vector<double, 2>::create(nani::static_array<double, 2>(2.4, 3.9));

  constexpr auto c = a + b;
  static_assert(nani::abs(c[0] - 10.7) < tol * a[0]);
  static_assert(nani::abs(c[1] - 8.9) < tol * a[1]);
}

TEST_CASE("operator -", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(8.3, 5.0));
  constexpr auto b = nani::vector<double, 2>::create(nani::static_array<double, 2>(2.4, 3.9));

  constexpr auto c = a - b;
  static_assert(nani::abs(c[0] - 5.9) < tol * a[0]);
  static_assert(nani::abs(c[1] - 1.1) < tol * a[1]);
}

TEST_CASE("inner product", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(8.3, 5.0));
  constexpr auto b = nani::vector<double, 2>::create(nani::static_array<double, 2>(2.4, 3.9));

  constexpr auto c = a * b;
  static_assert(nani::abs(c - 39.42) < tol * c);
}

TEST_CASE("scalar multiplication", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(8.3, 5.0));

  constexpr auto c = a * 2.0;
  static_assert(nani::abs(c[0] - 16.6) < tol * a[0]);
  static_assert(nani::abs(c[1] - 10.0) < tol * a[1]);
}

TEST_CASE("scalar division", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(8.3, 5.0));

  constexpr auto c = a / 2.0;
  static_assert(nani::abs(c[0] - 4.15) < tol * a[0]);
  static_assert(nani::abs(c[1] - 2.5) < tol * a[1]);
}

TEST_CASE("l2_norm", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(3.0, 4.0));

  constexpr auto c = nani::l2_norm(a);
  static_assert(nani::abs(c - 5.0) < tol * nani::l2_norm(a));
}

TEST_CASE("2d exterior product", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(8.3, 5.0));

  constexpr auto b = nani::vector<double, 2>::create(nani::static_array<double, 2>(2.4, 3.9));

  constexpr auto c = a ^ b;
  static_assert(nani::abs(c - 20.37) < tol * nani::abs(c));
}

TEST_CASE("normalize", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(3.0, 4.0));

  constexpr auto c = nani::normalize(a);
  static_assert(nani::abs(c[0] - 3.0 / 5.0) < tol * nani::l2_norm(c));
  static_assert(nani::abs(c[1] - 4.0 / 5.0) < tol * nani::l2_norm(c));
}

TEST_CASE("normal", "[all]")
{
  constexpr auto tol = std::numeric_limits<double>::epsilon();
  constexpr auto a = nani::vector<double, 2>::create(nani::static_array<double, 2>(3.0, 4.0));

  constexpr auto c = nani::normal(a);
  static_assert(nani::abs(c[0] + 4.0) < tol * nani::l2_norm(c));
  static_assert(nani::abs(c[1] - 3.0) < tol * nani::l2_norm(c));
}
