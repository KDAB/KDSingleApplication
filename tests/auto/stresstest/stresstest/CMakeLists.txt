set(stresstest_SRCS main.cpp)
add_executable(stresstest ${stresstest_SRCS})
target_link_libraries(stresstest
       Qt5::Widgets
       kdsingleapplication
)

