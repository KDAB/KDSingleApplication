# This file is part of KDSingleApplication.
#
# SPDX-FileCopyrightText: 2020 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(
    tst_simpletest
    ../tst_simpletest.cpp
)
target_link_libraries(
    tst_simpletest Qt::Test
)
add_test(NAME tst_simpletest COMMAND tst_simpletest)
