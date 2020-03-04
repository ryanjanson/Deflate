//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
// Copyright (c) 2016-2020 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//

// Test that header file is self-contained.
#include <boost/deflate/error.hpp>

#include "test_suite.hpp"

namespace boost {
namespace deflate {

class error_test
{
public:
    void check(char const* name, error ev)
    {
        auto const ec = make_error_code(ev);
        auto const& cat = make_error_code(
            static_cast<deflate::error>(0)).category();
        BOOST_TEST(std::string{ec.category().name()} == name);
        BOOST_TEST(! ec.message().empty());
        BOOST_TEST(
            std::addressof(ec.category()) == std::addressof(cat));
        BOOST_TEST(cat.equivalent(
            static_cast<std::underlying_type<error>::type>(ev),
                ec.category().default_error_condition(
                    static_cast<std::underlying_type<error>::type>(ev))));
        BOOST_TEST(cat.equivalent(ec,
            static_cast<std::underlying_type<error>::type>(ev)));
    }

    void run()
    {
        check("boost.deflate", error::need_buffers);
        check("boost.deflate", error::end_of_stream);
        check("boost.deflate", error::need_dict);
        check("boost.deflate", error::stream_error);

        check("boost.deflate", error::invalid_block_type);
        check("boost.deflate", error::invalid_stored_length);
        check("boost.deflate", error::too_many_symbols);
        check("boost.deflate", error::invalid_code_lengths);
        check("boost.deflate", error::invalid_bit_length_repeat);
        check("boost.deflate", error::missing_eob);
        check("boost.deflate", error::invalid_literal_length);
        check("boost.deflate", error::invalid_distance_code);
        check("boost.deflate", error::invalid_distance);

        check("boost.deflate", error::incorrect_header_check);
        check("boost.deflate", error::incorrect_data_check);
        check("boost.deflate", error::incorrect_length_check);
        check("boost.deflate", error::unknown_compression_method);
        check("boost.deflate", error::invalid_window_size);
        check("boost.deflate", error::header_crc_mismatch);

        check("boost.deflate", error::over_subscribed_length);
        check("boost.deflate", error::incomplete_length_set);

        check("boost.deflate", error::general);
    }
};

TEST_SUITE(error_test, "error");

} // deflate
} // boost
