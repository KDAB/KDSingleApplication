# This file is part of KDSingleApplication.
#
# SPDX-FileCopyrightText: 2020 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
set(widgetsingleapplication_SRCS main.cpp primaryinstancewidget.cpp secondaryinstancewidget.cpp)
add_executable(
    widgetsingleapplication
    ${widgetsingleapplication_SRCS}
)
target_link_libraries(
    widgetsingleapplication Qt${QT_VERSION_MAJOR}::Widgets kdsingleapplication
)
