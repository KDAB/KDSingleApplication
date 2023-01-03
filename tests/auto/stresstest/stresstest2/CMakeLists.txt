# This file is part of KDSingleApplication.
#
# SPDX-FileCopyrightText: 2020-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
set(stresstest2_SRCS main.cpp)
add_executable(
    stresstest2
    ${stresstest2_SRCS}
)
target_link_libraries(
    stresstest2 Qt5::Widgets kdsingleapplication
)
