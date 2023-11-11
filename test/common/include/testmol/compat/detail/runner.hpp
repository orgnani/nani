// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#ifndef TESTMOL_COMPAT_DETAIL_RUNNER_HPP
#define TESTMOL_COMPAT_DETAIL_RUNNER_HPP

#include <string>
namespace testmol::compat::detail {
auto runner(int argc, char** argv, std::string const& suffix) -> int;
} // namespace testmol::compat::detail

#endif // TESTMOL_COMPAT_DETAIL_RUNNER_HPP
