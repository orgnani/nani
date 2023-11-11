// -------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
// -------------------------------------------------------------------------------------------------

#ifndef TESTMOL_COMPAT_CATCH_MAIN_HPP
#define TESTMOL_COMPAT_CATCH_MAIN_HPP

#include <testmol/compat/detail/runner.hpp>

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TESTMOL_CATCH_MAIN(suffix)                                                                 \
                                                                                                   \
  auto main(int argc, char* argv[])->int                                                           \
  {                                                                                                \
    return testmol::compat::detail::runner(argc, argv, std::string(suffix));                       \
  }

#endif // TESTMOL_COMPAT_CATCH_MAIN_HPP
