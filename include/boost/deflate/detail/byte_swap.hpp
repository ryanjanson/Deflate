//
// Copyright (c) 2020 Vinnie Falco (ryand.janson@gmail.com)
// Copyright (c) 2016-2020 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_DEFLATE_DETAIL_BYTE_SWAP_HPP
#define BOOST_DEFLATE_DETAIL_BYTE_SWAP_HPP

namespace boost {
namespace deflate {
namespace detail {

  constexpr std::uint16_t bswap(std::uint16_t v) noexcept {
      return ((v & 0xff) << 8) | (v >> 8);
  }

  constexpr std::uint32_t bswap(std::uint32_t v) noexcept {
      return (static_cast<uint32_t>(bswap(static_cast<uint16_t>(v & 0xffff))) << 16) |
             bswap(static_cast<uint16_t>(v >> 16));
  }

  constexpr std::uint64_t bswap(std::uint64_t v) noexcept {
      return (static_cast<uint64_t>((bswap(static_cast<uint32_t>(v & 0xffffffff)))) << 32) |
             bswap(static_cast<uint32_t>(v >> 32));
  }

}
}
}


#endif //BOOST_DEFLATE_DETAIL_BYTE_SWAP_HPP
