# This file is part of KDSingleApplication.
#
# SPDX-FileCopyrightText: 2026 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
set(waitforprimaryexittest_SRCS main.cpp)
add_executable(
    waitforprimaryexittest
    ${waitforprimaryexittest_SRCS}
)
target_link_libraries(
    waitforprimaryexittest Qt::Widgets kdsingleapplication
)
