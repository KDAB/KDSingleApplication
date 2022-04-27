set(widgetsingleapplication_SRCS main.cpp primaryinstancewidget.cpp secondaryinstancewidget.cpp)
add_executable(widgetsingleapplication ${widgetsingleapplication_SRCS})
target_link_libraries(widgetsingleapplication
    Qt${QT_VERSION_MAJOR}::Widgets
    kdsingleapplication
)

