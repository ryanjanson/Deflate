//
// Copyright (c) Ryan Janson 2020 (ryand.janson@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//

#ifndef BOOST_DEFLATE_SRC_HPP
#define BOOST_DEFLATE_SRC_HPP

/*
This file is meant to be included once, in a translation unit of
the program, with the macro BOOST_DEFLATE_SEPARATE_COMPILATION defined.
*/

#define BOOST_DEFLATE_SOURCE

#include <boost/deflate/detail/config.hpp>

#if defined(BOOST_DEFLATE_HEADER_ONLY)
# error Do not compile Deflate library source with BOOST_DEFLATE_HEADER_ONLY defined
#endif

#include <boost/deflate/detail/adler.ipp>
#include <boost/deflate/detail/crc.ipp>
#include <boost/deflate/detail/easy.ipp>
#include <boost/deflate/detail/deflate_stream.ipp>
#include <boost/deflate/detail/inflate_stream.ipp>
#include <boost/deflate/impl/error.ipp>

#endif