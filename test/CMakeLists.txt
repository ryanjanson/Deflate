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

source_group ("" FILES include/boost/deflate deflate)

source_group (deflate FILES ${PROJECT_SOURCE_DIR}/include/boost/deflate.hpp)
source_group (TREE ${PROJECT_SOURCE_DIR}/include/boost/deflate PREFIX deflate FILES ${PROJECT_FILES})

add_subdirectory(extern)

add_executable (deflate-tests
        Jamfile
        main.cpp
        adler32.cpp
        crc32.cpp
        error.cpp
        deflate_stream.cpp
        inflate_stream.cpp
        zlib.cpp
        test_suite.hpp)

target_include_directories(deflate-tests PRIVATE extern/zlib-1.2.11)
target_link_libraries (deflate-tests PRIVATE boost_deflate zlib-test)

add_test(deflate-tests deflate-tests)
