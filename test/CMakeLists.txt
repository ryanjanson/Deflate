#
# Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
# Copyright (c) 2019-2020 Vinnie Falco (vinnie.falco@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/ryanjanson/deflate
#

include_directories (./extern)

GroupSources (include/boost/deflate deflate)
GroupSources (test/extern/zlib-1.2.11 zlib)
GroupSources (test/beast/zlib "/")

set (ZLIB_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/adler32.h
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/deflate.h
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/inffast.h
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/inffixed.h
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/inflate.h
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/inftrees.h
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/trees.h
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/zlib.h
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/zutil.h
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/adler32.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/compress.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/crc32.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/deflate.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/infback.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/inffast.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/inflate.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/inftrees.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/trees.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/uncompr.c
        ${CMAKE_CURRENT_LIST_DIR}/extern/zlib-1.2.11/zutil.c
        )

if (MSVC)
    set_source_files_properties (${ZLIB_SOURCES} PROPERTIES COMPILE_FLAGS "/wd4127 /wd4131 /wd4244")
endif()

source_group (deflate FILES ${PROJECT_SOURCE_DIR}/include/boost/deflate.hpp)
source_group (TREE ${PROJECT_SOURCE_DIR}/include/boost/deflate PREFIX deflate FILES ${PROJECT_FILES})

GroupSources(test "/")

add_definitions(-DBOOST_DEFLATE_HEADER_ONLY=1)

add_executable (deflate-tests
        ${BOOST_DEFLATE_FILES}
        ${ZLIB_SOURCES}
        Jamfile
        main.cpp
        adler32.cpp
        crc32.cpp
        error.cpp
        deflate_stream.cpp
        inflate_stream.cpp
        zlib.cpp
        test_suite.hpp)

add_test(deflate-tests deflate-tests)
