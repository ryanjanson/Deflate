//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//

#ifndef BOOST_DEFLATE_EASY_HPP
#define BOOST_DEFLATE_EASY_HPP

#include <boost/deflate/config.hpp>
#include <boost/deflate/deflate_stream.hpp>
#include <boost/deflate/inflate_stream.hpp>
#include <string>
#include <vector>

namespace boost {
namespace deflate {

BOOST_DEFLATE_DECL
optional<std::string> easy_compress(string_view in, wrap wrapping = boost::deflate::wrap::none);

BOOST_DEFLATE_DECL
optional<std::string> easy_uncompress(string_view in, wrap wrapping = boost::deflate::wrap::none);

}
}

#endif //BOOST_DEFLATE_EASY_HPP
