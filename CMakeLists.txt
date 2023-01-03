# This file is part of KDSingleApplication.
#
# SPDX-FileCopyrightText: 2020-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
cmake_minimum_required(VERSION 3.10)

project(
    KDSingleApplication
    VERSION 0.1
    DESCRIPTION "KDSingleApplication is a helper class for single-instance policy applications written by KDAB"
    HOMEPAGE_URL "https://github.com/KDAB/KDSingleApplication"
    LANGUAGES CXX
)

cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0042 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/ECM/modules")

find_package(Qt5Widgets)
find_package(Qt5Network)

set(CMAKE_AUTOMOC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(${PROJECT_NAME}_DEVELOPER_MODE "Developer Mode" OFF)
option(${PROJECT_NAME}_TESTS "Build the tests" OFF)
option(${PROJECT_NAME}_EXAMPLES "Build the examples" ON)
option(${PROJECT_NAME}_STATIC "Build lib as shared" OFF)

add_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_SIGNALS_SLOTS_KEYWORDS
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    -DQT_STRICT_ITERATORS
    -DQT_NO_KEYWORDS
    -DQT_DISABLE_DEPRECATED_BEFORE=0x051200
    -DQT_NO_FOREACH
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
    # Strict iterators can't be used on Windows with MSVC (for Qt<5.13) as they lead
    # to a link error when application code iterates over a QVector<QPoint> for instance,
    # unless Qt itself was also built with strict iterators.
    # See example at https://bugreports.qt.io/browse/AUTOSUITE-946
    add_definitions(-DQT_STRICT_ITERATORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wunused-parameter")
endif()

if(${PROJECT_NAME}_DEVELOPER_MODE)
    add_definitions(-DQT_FORCE_ASSERTS)
    if(NOT MSVC)
        add_compile_options(-Wall -Wextra -Werror)
    endif()
endif()

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(${PROJECT_NAME}_IS_ROOT_PROJECT TRUE)
else()
    #Always disable tests, examples, docs when used as a submodule
    set(${PROJECT_NAME}_IS_ROOT_PROJECT FALSE)
    set(${PROJECT_NAME}_TESTS FALSE)
    set(${PROJECT_NAME}_EXAMPLES FALSE)
endif()

add_subdirectory(src)
if(${PROJECT_NAME}_TESTS)
    enable_testing()
    add_definitions(-DKDSINGLEAPPLICATION_BINARY_DIR="${CMAKE_BINARY_DIR}/bin/")
    add_subdirectory(tests)
endif()
if(${PROJECT_NAME}_EXAMPLES)
    add_subdirectory(examples)
endif()

if(${PROJECT_NAME}_IS_ROOT_PROJECT)
    # Add uninstall target (not for submodules since parent projects typically have uninstall too)
    include(ECMUninstallTarget)
endif()
