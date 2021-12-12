cmake_minimum_required(VERSION 3.10)

project(KDSingleApplication
        VERSION 0.1
        DESCRIPTION "KDSingleApplication is a helper class for single-instance policy applications written by KDAB"
        HOMEPAGE_URL "https://github.com/KDAB/KDSingleApplication"
        LANGUAGES CXX)

cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0042 NEW)

find_package(Qt6 COMPONENTS Network)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS Network REQUIRED)
endif (NOT Qt6_FOUND)

set(CMAKE_AUTOMOC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(OPTION_DEVELOPER_MODE "Developer Mode" OFF)
option(${PROJECT_NAME}_TESTS "Build the tests" OFF)
option(${PROJECT_NAME}_EXAMPLES "Build the examples" OFF)
option(${PROJECT_NAME}_STATIC "Build lib as shared" OFF)

add_definitions(-DQT_NO_CAST_TO_ASCII
                -DQT_NO_CAST_FROM_ASCII
                -DQT_NO_URL_CAST_FROM_STRING
                -DQT_NO_CAST_FROM_BYTEARRAY
                -DQT_NO_SIGNALS_SLOTS_KEYWORDS
                -DQT_USE_QSTRINGBUILDER
                -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
                -DQT_STRICT_ITERATORS
                -DQT_NO_KEYWORDS
                -DQT_DISABLE_DEPRECATED_BEFORE=0x051200
                -DQT_NO_FOREACH)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT WIN32)
    # Strict iterators can't be used on Windows, they lead to a link error
    # when application code iterates over a QVector<QPoint> for instance, unless
    # Qt itself was also built with strict iterators.
    # See example at https://bugreports.qt.io/browse/AUTOSUITE-946
    add_definitions(-DQT_STRICT_ITERATORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-parameter")
endif()

if (OPTION_DEVELOPER_MODE)
    add_definitions(-DQT_FORCE_ASSERTS)
    if (NOT MSVC)
        add_compile_options(-Wall -Wextra -Werror)
    endif()
endif()

add_subdirectory(src)
if (${PROJECT_NAME}_TESTS)
    enable_testing()
    add_definitions(-DKDSINGLEAPPLICATION_BINARY_DIR="${CMAKE_BINARY_DIR}/bin/" )
    add_subdirectory(tests)
endif()
if (${PROJECT_NAME}_EXAMPLES)
    add_subdirectory(examples)
endif()
