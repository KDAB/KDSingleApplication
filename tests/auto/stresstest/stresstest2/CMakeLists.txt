set(stresstest2_SRCS main.cpp)
add_executable(stresstest2 ${stresstest2_SRCS})
target_link_libraries(stresstest2
       Qt5::Widgets
       kdsingleapplication
)

