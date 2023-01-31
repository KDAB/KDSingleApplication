# This file is part of KDSingleApplication.
#
# SPDX-FileCopyrightText: 2020-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
set(simpletest_SRCS main.cpp)
add_executable(
    simpletest
    ${simpletest_SRCS}
)
target_link_libraries(
    simpletest Qt::Widgets kdsingleapplication
)
