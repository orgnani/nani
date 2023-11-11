// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#ifndef NANI_WRAP_ITERATOR_HPP
#define NANI_WRAP_ITERATOR_HPP

#include <iterator>
#include <memory>
#include <type_traits>

namespace nani {
/** Wrap Iterator
 *
 * In contiguous arrays, iterators are just pointers to elements. However, for type safety reasons,
 * it makes sense to have different types for iterators and pointers. `wrap_iterator` takes an
 * iterator and wraps it in a dummy type that can be use as another iterator. This then provides
 * strong type-safety and checking in code where there is a possibility of iterators and pointers to
 * be mixed incorrectly.
 **/
template <typename Iter>
class wrap_iterator {

public:
  using iterator_type = Iter;

private:
  using iterator_traits = std::iterator_traits<iterator_type>;

public:
  using value_type = typename iterator_traits::value_type;
  using difference_type = typename iterator_traits::difference_type;
  using pointer = typename iterator_traits::pointer;
  using reference = typename iterator_traits::reference;
  using iterator_category = std::contiguous_iterator_tag;

private:
  iterator_type iter_;

public:
  template <typename U>
    requires std::is_convertible_v<U, Iter>
  constexpr explicit wrap_iterator(wrap_iterator<U> const& u) noexcept;

  constexpr explicit wrap_iterator(iterator_type i) noexcept;

public:
  constexpr auto operator*() const noexcept -> reference;

  constexpr auto operator->() const noexcept -> pointer;

  constexpr auto operator++() noexcept -> wrap_iterator&;

  constexpr auto operator--() noexcept -> wrap_iterator&;

  constexpr auto operator++(int) noexcept -> wrap_iterator;

  constexpr auto operator--(int) noexcept -> wrap_iterator;

  constexpr auto operator+=(difference_type n) noexcept -> wrap_iterator&;

  constexpr auto operator-=(difference_type n) noexcept -> wrap_iterator&;

  constexpr auto operator[](difference_type n) const noexcept -> reference;

  constexpr auto base() const noexcept -> iterator_type;
};

// Deduction Guides
template <typename Iter>
wrap_iterator(Iter a) -> wrap_iterator<Iter>;

// Reference: https://github.com/llvm/llvm-project/commit/4118858b4e4d072ac2ceef6cbc52088438781f39
// We actually do need both a one-template-parameter and a two-template-parameter version of all the
// comparison operators, because if we have only the heterogeneous two-parameter version, then `x >
// x` is ambiguous:
//
//    template<class T, class U> int f(S<T>, S<U>) { return 1; }
//    template<class T> int f(T, T) { return 2; }  // rel_ops
//    S<int> s; f(s,s);  // ambiguous between #1 and #2
//
// Adding the one-template-parameter version fixes the ambiguity:
//
//    template<class T, class U> int f(S<T>, S<U>) { return 1; }
//    template<class T> int f(T, T) { return 2; }  // rel_ops
//    template<class T> int f(S<T>, S<T>) { return 3; }
//    S<int> s; f(s,s);  // #3 beats both #1 and #2
template <typename Iter>
constexpr auto operator==(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right)
  -> bool;

template <typename Iter1, typename Iter2>
constexpr auto operator==(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool;

template <typename Iter>
constexpr auto operator!=(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right)
  -> bool;

template <typename Iter1, typename Iter2>
constexpr auto operator!=(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool;

template <typename Iter>
constexpr auto operator<(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right) -> bool;

template <typename Iter1, typename Iter2>
constexpr auto operator<(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool;

template <typename Iter>
constexpr auto operator<=(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right)
  -> bool;

template <typename Iter1, typename Iter2>
constexpr auto operator<=(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool;

template <typename Iter>
constexpr auto operator>(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right) -> bool;

template <typename Iter1, typename Iter2>
constexpr auto operator>(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool;

template <typename Iter>
constexpr auto operator>=(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right)
  -> bool;

template <typename Iter1, typename Iter2>
constexpr auto operator>=(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool;

template <typename Iter>
constexpr auto operator+(
  wrap_iterator<Iter> const& x, typename wrap_iterator<Iter>::difference_type n)
  -> wrap_iterator<Iter>;

template <typename Iter>
constexpr auto operator+(
  typename wrap_iterator<Iter>::difference_type n, wrap_iterator<Iter> const& x)
  -> wrap_iterator<Iter>;

template <typename Iter1, typename Iter2>
constexpr auto operator-(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right) ->
  typename wrap_iterator<Iter1>::difference_type;

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

template <typename Iter>
template <typename U>
  requires std::is_convertible_v<U, Iter>
constexpr wrap_iterator<Iter>::wrap_iterator(wrap_iterator<U> const& u) noexcept : iter_(u.base())
{
}

template <typename Iter>
constexpr wrap_iterator<Iter>::wrap_iterator(iterator_type i) noexcept : iter_(i)
{
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::operator*() const noexcept -> reference
{
  return *iter_;
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::operator->() const noexcept -> pointer
{
  return std::to_address(iter_);
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::operator++() noexcept -> wrap_iterator&
{
  ++iter_;
  return *this;
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::operator--() noexcept -> wrap_iterator&
{
  --iter_;
  return *this;
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::operator++(int) noexcept -> wrap_iterator
{
  auto tmp = wrap_iterator(*this);
  ++(*this);
  return tmp;
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::operator--(int) noexcept -> wrap_iterator
{
  auto tmp = wrap_iterator(*this);
  --(*this);
  return tmp;
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::operator+=(difference_type n) noexcept -> wrap_iterator&
{
  iter_ += n;
  return *this;
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::operator-=(difference_type n) noexcept -> wrap_iterator&
{
  iter_ -= n;
  return *this;
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::operator[](difference_type n) const noexcept -> reference
{
  return iter_[n];
}

template <typename Iter>
constexpr auto wrap_iterator<Iter>::base() const noexcept -> iterator_type
{
  return iter_;
}

template <typename Iter>
constexpr auto operator==(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right) -> bool
{
  return left.base() == right.base();
}

template <typename Iter1, typename Iter2>
constexpr auto operator==(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool
{
  return left.base() == right.base();
}

template <typename Iter>
constexpr auto operator!=(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right) -> bool
{
  return (not(left == right));
}

template <typename Iter1, typename Iter2>
constexpr auto operator!=(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool
{
  return left != right;
}

template <typename Iter>
constexpr auto operator<(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right) -> bool
{
  return left.base() < right.base();
}

template <typename Iter1, typename Iter2>
constexpr auto operator<(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool
{
  return left.base() < right.base();
}

template <typename Iter>
constexpr auto operator<=(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right) -> bool
{
  return left.base() <= right.base();
}

template <typename Iter1, typename Iter2>
constexpr auto operator<=(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool
{
  return left.base() <= right.base();
}

template <typename Iter>
constexpr auto operator>(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right) -> bool
{
  return left.base() > right.base();
}

template <typename Iter1, typename Iter2>
constexpr auto operator>(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool
{
  return left.base() > right.base();
}

template <typename Iter>
constexpr auto operator>=(wrap_iterator<Iter> const& left, wrap_iterator<Iter> const& right) -> bool
{
  return left.base() >= right.base();
}

template <typename Iter1, typename Iter2>
constexpr auto operator>=(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right)
  -> bool
{
  return left.base() >= right.base();
}

template <typename Iter>
constexpr auto operator+(
  wrap_iterator<Iter> const& x, typename wrap_iterator<Iter>::difference_type n)
  -> wrap_iterator<Iter>
{
  auto result = x;
  return result += n;
}

template <typename Iter>
constexpr auto operator+(
  typename wrap_iterator<Iter>::difference_type n, wrap_iterator<Iter> const& x)
  -> wrap_iterator<Iter>
{
  auto result = x;
  return result += n;
}

template <typename Iter1, typename Iter2>
constexpr auto operator-(wrap_iterator<Iter1> const& left, wrap_iterator<Iter2> const& right) ->
  typename wrap_iterator<Iter1>::difference_type
{
  return left.base() - right.base();
}

} // namespace nani

#endif // NANI_WRAP_ITERATOR_HPP
