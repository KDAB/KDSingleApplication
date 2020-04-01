include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
find_package(Qt5Test)

add_executable(tst_simpletest ../tst_simpletest.cpp)
target_link_libraries(tst_simpletest Qt5::Test)
add_test(NAME tst_simpletest COMMAND tst_simpletest)

