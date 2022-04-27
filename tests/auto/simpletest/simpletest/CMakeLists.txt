set(simpletest_SRCS main.cpp)
add_executable(simpletest ${simpletest_SRCS})
target_link_libraries(simpletest
    Qt${QT_VERSION_MAJOR}::Core
    kdsingleapplication
)
