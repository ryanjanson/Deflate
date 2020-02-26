//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
// Copyright (c) 2016-2020 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//
// This is a derivative work based on Zlib, copyright below:
/*
    Copyright (C) 1995-2013 Jean-loup Gailly and Mark Adler

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.

    Jean-loup Gailly        Mark Adler
    jloup@gzip.org          madler@alumni.caltech.edu

    The data format used by the zlib library is described by RFCs (Request for
    Comments) 1950 to 1952 in the files http://tools.ietf.org/html/rfc1950
    (zlib format), rfc1951 (deflate format) and rfc1952 (gzip format).
*/
#ifndef BOOST_DEFLATE_DETAIL_ADLER_IPP
#define BOOST_DEFLATE_DETAIL_ADLER_IPP

namespace boost {
namespace deflate {
namespace detail {

// zlib's Adler-32 implementation
unsigned adler32(const unsigned char* buf, unsigned len, unsigned adler) noexcept {
  constexpr auto base = 65521U; // largest prime smaller than 65536
  constexpr auto nmax = 5552; // max(n) such that
                              // 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1

  // split into component sum
  unsigned long sum = (adler >> 16u) & 0xffff;
  adler &= 0xffff;

  if (buf == nullptr)
      return 1U;

  if (len < 16) {
      while (len--) {
         adler += *buf++;
         sum += adler;
      }
      if (adler >= base)
         adler -= base;
      sum %= base;
      return adler | (sum << 16);
  }

  while (len >= nmax) {
      len -= nmax;
      auto n = nmax / 16;
      do {
          for(int i = 0; i < 16; ++i) {
              adler += (buf)[i];
              sum += adler;
          }
          buf += 16;
      } while (--n);
      adler %= base;
      sum %= base;
  }

  if (len) {
      while (len >= 16) {
          len -= 16;
          for(int i = 0; i < 16; ++i) {
              adler += (buf)[i];
              sum += adler;
          }
          buf += 16;
      }
      while (len--) {
          adler += *buf++;
          sum += adler;
      }
      adler %= base;
      sum %= base;
  }

  return adler | (sum << 16);
}

} // detail
} // deflate
} // boost

#endif