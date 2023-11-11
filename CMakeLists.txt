# --------------------------------------------------------------------------------------------------
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
# --------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.26)
project(nani VERSION 0.0.0.0)


find_package(Catch2 REQUIRED)
include(cmake/bs.cmake)

add_library(libnani)
set_target_properties(libnani PROPERTIES OUTPUT_NAME nani)

add_subdirectory(src)

if(BUILD_TESTING)
  include(CTest)
  add_subdirectory(test)
endif()
