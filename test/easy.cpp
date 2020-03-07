//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//

// Test that header file is self-contained.
#include <boost/deflate/easy.hpp>

#include "test_suite.hpp"

namespace boost {
namespace deflate {

class easy_test {
public:
  static void test_compress() {
      BOOST_TEST(easy_compress(""));
      BOOST_TEST(easy_compress("aaaaaaaaaaaaaaa"));
      BOOST_TEST(easy_compress("This is a test string"));
  }

  static void test_uncompress() {
      BOOST_TEST(easy_uncompress(""));
      BOOST_TEST(easy_uncompress("\x00\x05\x00\xfa\xff\x48\x65\x6c\x6c\x6f\x00\x00"));
      BOOST_TEST(!easy_uncompress("This is a test string"));
  }

  void run() {
      test_compress();
      test_uncompress();
  }
};

TEST_SUITE(easy_test, "easy");

} // deflate
} // boost