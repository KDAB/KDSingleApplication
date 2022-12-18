include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
find_package(Qt5Test)

add_executable(tst_stresstest ../tst_stresstest.cpp)
target_link_libraries(tst_stresstest Qt5::Test)
add_test(NAME tst_stresstest COMMAND tst_stresstest)
