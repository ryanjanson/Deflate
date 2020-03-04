//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
// Copyright (c) 2016-2020 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//

#include <boost/deflate/detail/adler.hpp>
#include "test_suite.hpp"

#include "zlib-1.2.11/zlib.h"

namespace boost {
namespace deflate {
namespace detail {

class adler32_test {
public:
  std::uint32_t eq(const char* data, int size = -1, std::uint32_t init = -1) {
    return adler32(reinterpret_cast<const unsigned char*>(data),
                   size > 0 ? size : std::strlen(data), init != -1U ? init : adler32(nullptr, 0))
           == ::adler32(init != -1U ? init : ::adler32(0, nullptr, 0),
                        reinterpret_cast<const unsigned char*>(data),
                        size > 0 ? size : std::strlen(data));
  }
  void run() {

        BOOST_TEST(eq(""));
        BOOST_TEST(eq("short"));
        BOOST_TEST(eq("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_"));
        BOOST_TEST(eq("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec vitae justo orci." \
                        " Curabitur mattis venenatis tortor, et posuere dolor scelerisque quis. Proin in " \
                        "commodo ipsum. Aenean ullamcorper nibh non convallis rhoncus. Nam auctor, eros e" \
                        "get accumsan mollis, risus nulla commodo sapien, et accumsan eros nisl a leo. Nu" \
                        "nc lobortis, nisi et consequat aliquam, justo purus ultrices ligula, non condime" \
                        "ntum diam purus vitae tellus. Proin quis ullamcorper magna. Integer scelerisque " \
                        "metus nec justo ultricies ultricies. Sed purus urna, euismod quis nisi nec, port" \
                        "a finibus felis. Duis leo lacus, feugiat et malesuada ac, posuere ut metus. Viva" \
                        "mus dictum leo id semper posuere. Curabitur id nibh vel elit efficitur euismod s" \
                        "ed vitae nulla. Proin erat mi, gravida at suscipit non, rhoncus vitae nibh. Maec" \
                        "enas cursus maximus leo eu tristique."));
        const char large[8000] {'\xff'};
        BOOST_TEST(eq(large, 8000));
        BOOST_TEST(eq("\x15\xb0"));
        BOOST_TEST(eq("\x02", 1, 0xfff0));

  }
};

TEST_SUITE(adler32_test, "adler32");

} // detail
} // deflate
} // boost