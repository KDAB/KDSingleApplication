include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(simplesingleapp main.cpp)
target_link_libraries(simplesingleapp Qt5::Widgets kdsingleapplication)

