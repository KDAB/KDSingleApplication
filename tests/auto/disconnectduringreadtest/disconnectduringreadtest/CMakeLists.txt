# This file is part of KDSingleApplication.
#
# SPDX-FileCopyrightText: 2020 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
set(disconnectduringreadtest_SRCS main.cpp)
add_executable(
    disconnectduringreadtest
    ${disconnectduringreadtest_SRCS}
)
target_link_libraries(
    disconnectduringreadtest Qt::Widgets kdsingleapplication
)
