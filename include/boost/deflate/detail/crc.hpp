//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
// Copyright (c) 2016-2020 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//


#ifndef BOOST_DEFLATE_DETAIL_CRC_HPP
#define BOOST_DEFLATE_DETAIL_CRC_HPP

#include <boost/deflate/config.hpp>
#include <cstdint>

namespace boost {
namespace deflate {
namespace detail {

/* Returns the CRC32 checksum of the next `size` bytes in `buf` with `crc` as
   an initial value. */
BOOST_DEFLATE_DECL
unsigned crc32(const unsigned char* buf,
               size_t size,
               std::uint32_t crc = 0) noexcept;

/* Applies CRC32 on the `I` lower bytes of `v` using `crc` as the crc value and
   output */
template <class T, int I = sizeof(T)>
void crc32_integral(T v, std::uint32_t& crc) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    std::array<unsigned char, I> arr;
    for(int i = 0; i < I; ++i)
        arr[i] = (v >> (i * 8));
    crc = crc32(arr.data(), I, crc);
}

} // detail
} // deflate
} // boost

#ifdef BOOST_DEFLATE_HEADER_ONLY
#include <boost/deflate/detail/crc.ipp>
#endif // BOOST_DEFLATE_HEADER_ONLY

#endif // BOOST_DEFLATE_DETAIL_CRC_HPP
