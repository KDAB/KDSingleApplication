# This file is part of KDSingleApplication.
#
# SPDX-FileCopyrightText: 2020 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
set(namelengthtest_SRCS main.cpp)
add_executable(
    namelengthtest
    ${namelengthtest_SRCS}
)
target_link_libraries(
    namelengthtest Qt::Widgets kdsingleapplication
)
