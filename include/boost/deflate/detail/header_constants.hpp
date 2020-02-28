//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
// Copyright (c) 2016-2020 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate

#ifndef BOOST_DEFLATE_DETAIL_HEADER_CONSTANTS_HPP
#define BOOST_DEFLATE_DETAIL_HEADER_CONSTANTS_HPP

namespace boost {
namespace deflate {
namespace detail {

// zlib compression method and info
enum z_cmf : unsigned char {
    ZMTH_DEFLATED  = 8,
    ZMTH_RESERVED  = 15,
    ZMTH_MASK      = 0x0f,
    ZMTH_MASK_TZ   = 0, // trailing zeroes of the method mask
    ZCINFO_MASK    = 0xf0,
    ZCINFO_MASK_TZ = 4 // trailing zeroes of the info mask
};

// zlib header flags
enum z_flags : unsigned char {
    ZFLG_CHECK_MASK    = 0x1f,
    ZFLG_CHECK_MASK_TZ = 0,  // trailing zeroes of the check mask
    ZFLG_CHECK_FACTOR  = 31,
    ZFLG_DICT          = 0x20,
    ZFLG_LEVEL_MASK    = 0xc0,
    ZFLG_LEVEL_MASK_TZ = 6  // trailing zeroes of the level mask
};

// gzip header flags
enum gz_flags : unsigned char {
    FTEXT    = 1u << 0,
    FHCRC    = 1u << 1,
    FEXTRA   = 1u << 2,
    FNAME    = 1u << 3,
    FCOMMENT = 1u << 4,
    RESERVED_FLAGS  = 0xe0,
};

// gzip compression method
enum gz_method : unsigned char {
    DEFLATE  = 8,
    RESERVED_METHODS = 0x7,
};

}
}
}

#endif //BOOST_DEFLATE_DETAIL_INFLATE_STREAM_HPP
