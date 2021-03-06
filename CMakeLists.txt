#
# Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
# Copyright (c) 2019-2020 Vinnie Falco (vinnie.falco@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/ryanjanson/deflate
#

cmake_minimum_required(VERSION 3.5...3.16)

set(BOOST_DEFLATE_VERSION 1)
if(BOOST_SUPERPROJECT_VERSION)
    set(BOOST_DEFLATE_VERSION ${BOOST_SUPERPROJECT_VERSION})
endif()

project(boost_deflate VERSION ${BOOST_DEFLATE_VERSION} LANGUAGES CXX)

file(GLOB_RECURSE BOOST_DEFLATE_HEADERS CONFIGURE_DEPENDS
        include/boost/*.hpp
        include/boost/*.ipp
        )

set(BOOST_DEFLATE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/src.cpp
        )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(${CMAKE_VERSION} VERSION_GREATER 3.7.2)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/boost PREFIX "" FILES ${BOOST_DEFLATE_HEADERS})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "" FILES ${BOOST_DEFLATE_SOURCES})
endif()

add_library(boost_deflate ${BOOST_DEFLATE_HEADERS} ${BOOST_DEFLATE_SOURCES})
add_library(Boost::deflate ALIAS boost_deflate)

target_compile_features(boost_deflate PUBLIC cxx_constexpr)

target_include_directories(boost_deflate PUBLIC include)

target_compile_definitions(boost_deflate PUBLIC BOOST_DEFLATE_NO_LIB=1)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(boost_deflate PUBLIC BOOST_DEFLATE_DYN_LINK=1)
else()
    target_compile_definitions(boost_deflate PUBLIC BOOST_DEFLATE_STATIC_LINK=1)
endif()

option(BOOST_DEFLATE_STANDALONE "Build boost::deflate as a static standalone library" FALSE)

if(BOOST_DEFLATE_STANDALONE)
    target_compile_features(boost_deflate PUBLIC cxx_std_17)
    target_compile_definitions(boost_deflate PUBLIC BOOST_DEFLATE_STANDALONE)
    add_subdirectory(example)
elseif(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    #
    # Root project such as on Travis or other CI,
    # or when producing Visual Studio Solution and Projects
    #

    if (BOOST_DEFLATE_FIND_BOOST)
        find_package(Boost REQUIRED COMPONENTS system)
        target_link_libraries(boost_deflate PUBLIC Boost::system)
    else()
        get_filename_component(BOOST_ROOT ../.. ABSOLUTE)
        target_include_directories(boost_deflate PUBLIC ${BOOST_ROOT})
        target_link_directories(boost_deflate PUBLIC ${BOOST_ROOT}/stage/lib)
    endif()

    #add_subdirectory(bench)
    add_subdirectory(example)
    add_subdirectory(test)

elseif(BOOST_SUPERPROJECT_VERSION)
    #
    # Building with Boost superproject cmake
    #
    target_link_libraries(boost_deflate
            PUBLIC
            Boost::assert
            Boost::config
            Boost::core
            Boost::exception
            Boost::system
            Boost::utility
            )
else()
    #
    # Out-of-tree, non-standalone build
    #
    find_package(Boost COMPONENTS system)
    target_link_libraries(boost_deflate
            PUBLIC
            Boost::system
            )
    option(BUILD_TESTING "Build the tests" ON)
    if (BUILD_TESTING)
        #add_subdirectory(bench)
        add_subdirectory(example)
        add_subdirectory(test)
    endif()

endif()