// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#include <catch2/catch_session.hpp>
#include <testmol/compat/detail/runner.hpp>

namespace testmol::compat::detail {
auto runner(
  [[maybe_unused]] int argc,
  [[maybe_unused]] char** argv,
  [[maybe_unused]] std::string const& suffix) -> int
{
  Catch::Session session;
  return session.run();
}

} // namespace testmol::compat::detail
