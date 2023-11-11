// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#ifndef NANI_META_HPP
#define NANI_META_HPP

namespace nani::meta {
constexpr auto is_debug_build() noexcept -> bool
{
#ifdef NANI_DEBUG_BUILD
  return true;
#else
  return false;
#endif
}
} // namespace nani::meta

#endif // NANI_META_HPP
