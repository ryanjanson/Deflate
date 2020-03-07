//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//


#ifndef BOOST_DEFLATE_DETAIL_EASY_HPP
#define BOOST_DEFLATE_DETAIL_EASY_HPP

#include <boost/deflate/easy.hpp>

namespace boost {
namespace deflate {

optional<std::string> easy_compress(string_view in, wrap wrapping) {
    constexpr static auto growth_factor = 1.5f;
    std::string out{};
    deflate_stream ds{};
    out.resize(ds.upper_bound(in.size()));
    ds.reset(6, 15, 8, Strategy::normal);
    error_code ec;

    z_params zp{};
    zp.next_in = &in[0];
    zp.next_out = &out[0];
    zp.avail_in = in.size();
    zp.avail_out = out.size();

    while(zp.avail_in > 0) {
        ds.write(zp, Flush::full, ec);
        if(ec && ec != error::need_buffers)
            return {};

        out.resize(out.size() * growth_factor);
        zp.next_out = &out[zp.total_out];
        zp.avail_out = out.size();
    }

    if(ec)
        return {};
    return out;
}

optional<std::string> easy_uncompress(string_view in, wrap wrapping) {
    constexpr static auto growth_factor = 2.f;
    std::string out{};
    inflate_stream is{};
    out.resize(in.size() * 2);
    is.reset(15, wrapping, true);
    error_code ec;

    z_params zp{};
    zp.next_in = &in[0];
    zp.next_out = &out[0];
    zp.avail_in = in.size();
    zp.avail_out = out.size();

    while(zp.avail_in > 0) {
        is.write(zp, Flush::full, ec);
        if(ec && ec != error::need_buffers)
            return {};

        out.resize(out.size() * growth_factor);
        zp.next_out = &out[zp.total_out];
        zp.avail_out = out.size();
    }
    return out;
}

} // beast
} // boost

#endif  //BOOST_DEFLATE_DETAIL_EASY_HPP