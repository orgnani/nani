# --------------------------------------------------------------------------------------------------
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
# --------------------------------------------------------------------------------------------------

find_package(Catch2 REQUIRED)

add_library(libtestnani)
set_target_properties(libtestnani PROPERTIES OUTPUT_NAME testnani)

add_subdirectory(common)

unit_test_directory(unit libtestnani "nani" "production")

target_link_libraries(libtestnani PUBLIC Catch2::Catch2)
target_link_libraries(libtestnani PUBLIC libnani)
