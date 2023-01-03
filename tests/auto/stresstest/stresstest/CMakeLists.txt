# This file is part of KDSingleApplication.
#
# SPDX-FileCopyrightText: 2020-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
set(stresstest_SRCS main.cpp)
add_executable(
    stresstest
    ${stresstest_SRCS}
)
target_link_libraries(
    stresstest Qt5::Widgets kdsingleapplication
)
