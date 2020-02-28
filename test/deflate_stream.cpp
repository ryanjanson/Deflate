//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
// Copyright (c) 2016-2020 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//

// Test that header file is self-contained.
#include <boost/deflate/deflate_stream.hpp>
#include <boost/deflate/deflate.hpp>

#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <random>
#include <string>

#include "test_suite.hpp"
#include "zlib-1.2.11/zlib.h"

namespace boost {
namespace deflate {

class deflate_stream_test
{
    struct ICompressor {
        virtual void init() = 0;
        virtual void init(
            int level,
            int windowBits,
            int memLevel,
            int strategy) = 0;

        virtual std::size_t avail_in() const noexcept = 0;
        virtual void avail_in(std::size_t) noexcept = 0;
        virtual void const* next_in() const noexcept = 0;
        virtual void next_in(const void*) noexcept = 0;
        virtual std::size_t avail_out() const noexcept = 0;
        virtual void avail_out(std::size_t) noexcept = 0;
        virtual void* next_out() const noexcept = 0;
        virtual void next_out(void*) noexcept = 0;
        virtual std::size_t total_out() const noexcept = 0;

        virtual std::size_t bound(std::size_t) = 0;
        virtual error_code write(Flush) = 0;
        virtual ~ICompressor() = default;
    };
    class ZlibCompressor : public ICompressor {
        z_stream zs{};

    public:
        ZlibCompressor() = default;
        void init() override {
            deflateEnd(&zs);
            zs = {};
            const auto res = deflateInit2(&zs, -1, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
            if(res != Z_OK)
               throw std::invalid_argument{"zlib compressor: failure"};
        }
        void init(
            int level,
            int windowBits,
            int memLevel,
            int strategy) override
        {
            deflateEnd(&zs);
            zs = {};
            const auto res = deflateInit2(&zs, level, Z_DEFLATED, -windowBits, memLevel, strategy);
            if(res != Z_OK)
               BOOST_THROW_EXCEPTION(std::invalid_argument{"zlib compressor: bad arg"});
        }

        virtual std::size_t avail_in() const noexcept override  { return zs.avail_in; }
        virtual void avail_in(std::size_t n) noexcept override { zs.avail_in = n; }
        virtual void const* next_in() const noexcept override { return zs.next_in; }
        virtual void next_in(const void* ptr) noexcept override { zs.next_in = const_cast<Bytef*>(static_cast<const Bytef*>(ptr)); }
        virtual std::size_t avail_out() const noexcept override { return zs.avail_out; }
        virtual void avail_out(std::size_t n_out) noexcept override { zs.avail_out = n_out; }
        virtual void* next_out() const noexcept override { return zs.next_out; }
        virtual void next_out(void* ptr) noexcept override { zs.next_out = (Bytef*)ptr; }
        virtual std::size_t total_out() const noexcept override { return zs.total_out; }

        std::size_t bound(std::size_t src_size) override {
           return deflateBound(&zs, static_cast<uLong>(src_size));
        }
        error_code write(Flush flush) override {
            constexpr static int zlib_flushes[] = {0, Z_BLOCK, Z_PARTIAL_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_TREES};
            const auto zlib_flush = zlib_flushes[static_cast<int>(flush)];
            if(zs.next_in == nullptr && zs.avail_in != 0)
              BOOST_THROW_EXCEPTION(std::invalid_argument{"zlib compressor: invalid input"});
            const auto res = ::deflate(&zs, zlib_flush);
            switch(res){
            case Z_OK:
                return {};
            case Z_STREAM_END:
                return error::end_of_stream;
            case Z_STREAM_ERROR:
                return error::stream_error;
            case Z_BUF_ERROR:
                return error::need_buffers;
            default:
                throw;
            }
        }

        ~ZlibCompressor() override {
            deflateEnd(&zs);
        }
    } zlib_compressor;
    class BoostCompressor : public ICompressor {
        z_params zp;
        deflate_stream ds;

    public:
        BoostCompressor() = default;
        void init() override {
          zp = {};
          ds.clear();
          ds.reset();
        }
        void init(
            int level,
            int windowBits,
            int memLevel,
            int strategy) override
        {
            zp = {};
            ds.clear();
            ds.reset(
                level,
                windowBits,
                memLevel,
                toStrategy(strategy));
        }

        virtual std::size_t avail_in() const noexcept override  { return zp.avail_in; }
        virtual void avail_in(std::size_t n) noexcept override { zp.avail_in = n; }
        virtual void const* next_in() const noexcept override { return zp.next_in; }
        virtual void next_in(const void* ptr) noexcept override { zp.next_in = ptr; }
        virtual std::size_t avail_out() const noexcept override { return zp.avail_out; }
        virtual void avail_out(std::size_t n_out) noexcept override { zp.avail_out = n_out; }
        virtual void* next_out() const noexcept override { return zp.next_out; }
        virtual void next_out(void* ptr) noexcept override { zp.next_out = (Bytef*)ptr; }
        virtual std::size_t total_out() const noexcept override { return zp.total_out; }

        std::size_t bound(std::size_t src_size) override {
            return ds.upper_bound(src_size);
        }
        error_code write(Flush flush) override {
          error_code ec{};
          ds.write(zp, flush, ec);
          return ec;
        }

        ~BoostCompressor() override = default;
    } beast_compressor;

public:
    // Lots of repeats, limited char range
    static
    std::string corpus1(std::size_t n)
    {
        static std::string const alphabet{
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        };
        std::string s;
        s.reserve(n + 5);
        std::mt19937 g;
        std::uniform_int_distribution<std::size_t> d0{
            0, alphabet.size() - 1};
        std::uniform_int_distribution<std::size_t> d1{
            1, 5};
        while(s.size() < n)
        {
            auto const rep = d1(g);
            auto const ch = alphabet[d0(g)];
            s.insert(s.end(), rep, ch);
        }
        s.resize(n);
        return s;
    }

    // Random data
    static
    std::string corpus2(std::size_t n)
    {
        std::string s;
        s.reserve(n);
        std::mt19937 g;
        std::uniform_int_distribution<std::uint32_t> d0{0, 255};
        while(n--)
            s.push_back(static_cast<char>(d0(g)));
        return s;
    }

    static
    std::string compress(
        string_view const& in,
        int level,                  // 0=none, 1..9, -1=default
        int windowBits,             // 9..15
        int memLevel)               // 1..9 (8=default)
    {
        int const strategy = Z_DEFAULT_STRATEGY;
        int result;
        z_stream zs;
        memset(&zs, 0, sizeof(zs));
        result = deflateInit2(
            &zs,
            level,
            Z_DEFLATED,
            -windowBits,
            memLevel,
            strategy);
        if(result != Z_OK)
            throw std::logic_error{"deflateInit2 failed"};
        zs.next_in = (Bytef*)in.data();
        zs.avail_in = static_cast<uInt>(in.size());
        std::string out;
        out.resize(deflateBound(&zs,
            static_cast<uLong>(in.size())));
        zs.next_in = (Bytef*)in.data();
        zs.avail_in = static_cast<uInt>(in.size());
        zs.next_out = (Bytef*)&out[0];
        zs.avail_out = static_cast<uInt>(out.size());
        result = ::deflate(&zs, Z_FULL_FLUSH);
        if(result != Z_OK)
            throw std::logic_error("deflate failed");
        out.resize(zs.total_out);
        deflateEnd(&zs);
        return out;
    }

    static
    std::string decompress(string_view const& in, Wrap wrap = Wrap::none)
    {
        int result;
        std::string out;
        z_stream zs;
        memset(&zs, 0, sizeof(zs));
        result = inflateInit2(&zs, wrap == Wrap::none ? -15 : 15);
        if(result != Z_OK)
            throw std::logic_error{"inflateInit2 failed"};
        try
        {
            zs.next_in = (Bytef*)in.data();
            zs.avail_in = static_cast<uInt>(in.size());
            for(;;)
            {
                out.resize(zs.total_out + 1024);
                zs.next_out = (Bytef*)&out[zs.total_out];
                zs.avail_out = static_cast<uInt>(
                    out.size() - zs.total_out);
                result = inflate(&zs, Z_SYNC_FLUSH);
                if( result == Z_NEED_DICT ||
                    result == Z_DATA_ERROR ||
                    result == Z_MEM_ERROR)
                {
                    throw std::logic_error("inflate failed");
                }
                if(zs.avail_out > 0)
                    break;
                if(result == Z_STREAM_END)
                    break;
            }
            out.resize(zs.total_out);
            inflateEnd(&zs);
        }
        catch(...)
        {
            inflateEnd(&zs);
            throw;
        }
        return out;
    }

    //--------------------------------------------------------------------------

    using self = deflate_stream_test;
    typedef void(self::*pmf_t)(
        ICompressor& c,
        int level, int windowBits, int memLevel,
        int strategy, std::string const&);

    static
    Strategy toStrategy(int strategy)
    {
        switch(strategy)
        {
        default:
        case 0: return Strategy::normal;
        case 1: return Strategy::filtered;
        case 2: return Strategy::huffman;
        case 3: return Strategy::rle;
        case 4: return Strategy::fixed;
        }
    }

    void doDeflate1_beast(
        ICompressor& c,
        int level, int windowBits, int memLevel,
        int strategy, std::string const& check)
    {
        std::string out;
        c.init(
            level,
            windowBits,
            memLevel,
            strategy);
        out.resize(c.bound(check.size()));
        c.next_in(check.data());
        c.avail_in(check.size());
        c.next_out((void*)out.data());
        c.avail_out(out.size());
        {
            bool progress = true;
            for(;;)
            {
                error_code ec = c.write(Flush::full);
                if( ec == error::need_buffers ||
                    ec == error::end_of_stream) // per zlib FAQ
                    goto fin;
                if(! BOOST_TESTS(! ec, ec.message().c_str()))
                    goto err;
                if(! BOOST_TEST(progress))
                    goto err;
                progress = false;
            }
        }

    fin:
        out.resize(c.total_out());
        BOOST_TEST(decompress(out) == check);

    err:
        ;
    }

    //--------------------------------------------------------------------------

    void doDeflate2_beast(
        ICompressor& c,
        int level, int windowBits, int memLevel,
        int strategy, std::string const& check)
    {
        for(std::size_t i = 1; i < check.size(); ++i)
        {
            for(std::size_t j = 1;; ++j)
            {
                c.init(
                    level,
                    windowBits,
                    memLevel,
                    strategy);
                std::string out;
                out.resize(c.bound(check.size()));
                if(j >= out.size())
                    break;
                c.next_in((void*)check.data());
                c.avail_in(i);
                c.next_out((void*)out.data());
                c.avail_out(j);
                bool bi = false;
                bool bo = false;
                for(;;)
                {
                    error_code ec = c.write(
                        bi ? Flush::full : Flush::none);
                    if( ec == error::need_buffers ||
                        ec == error::end_of_stream) // per zlib FAQ
                        goto fin;
                    if(! BOOST_TESTS(! ec, ec.message().c_str()))
                        goto err;
                    if(c.avail_in() == 0 && ! bi)
                    {
                        bi = true;
                        c.avail_in(check.size() - i);
                    }
                    if(c.avail_out() == 0 && ! bo)
                    {
                        bo = true;
                        c.avail_out(out.size() - j);
                    }
                }

            fin:
                out.resize(c.total_out());
                BOOST_TEST(decompress(out) == check);

            err:
                ;
            }
        }
    }

    //--------------------------------------------------------------------------

    void doMatrix(ICompressor& c, std::string const& check, pmf_t pmf)
    {
        for(int level = 0; level <= 9; ++level)
        {
            for(int windowBits = 8; windowBits <= 9; ++windowBits)
            {
                // zlib has a bug with windowBits==8
                if(windowBits == 8)
                    continue;
                for(int strategy = 0; strategy <= 4; ++strategy)
                {
                    for (int memLevel = 8; memLevel <= 9; ++memLevel)
                    {
                        (this->*pmf)(
                            c, level, windowBits, memLevel, strategy, check);
                    }
                }
            }
        }

        // Check default settings
        (this->*pmf)(c, compression::default_size, 15, 8, 0, check);
    }

    void testDeflate(ICompressor& c)
    {
        doMatrix(c, "Hello, world!", &self::doDeflate1_beast);
        doMatrix(c, "Hello, world!", &self::doDeflate2_beast);
        doMatrix(c, corpus1(56), &self::doDeflate2_beast);
        doMatrix(c, corpus1(1024), &self::doDeflate1_beast);
    }

    static
    void testInvalidSettings(ICompressor& c)
    {
        BOOST_TEST_THROWS(
            c.init(-42, 15, 8, static_cast<int>(Strategy::normal)), 
            std::invalid_argument);
        BOOST_TEST_THROWS(
            c.init(compression::default_size, -1, 8, static_cast<int>(Strategy::normal)),
            std::invalid_argument);
        BOOST_TEST_THROWS(
            c.init(compression::default_size, 15, -1, static_cast<int>(Strategy::normal)),
            std::invalid_argument);

        BOOST_TEST_THROWS(
            [&]()
            {
                c.init();
                c.avail_in(1);
                c.next_in(nullptr);
                c.write(Flush::full);
            }(), std::invalid_argument);
    }

    static
    void testWriteAfterFinish(ICompressor& c)
    {
        c.init();
        std::string out;
        out.resize(1024);
        string_view s = "Hello";
        c.next_in(s.data());
        c.avail_in(s.size());
        c.next_out(&out.front());
        c.avail_out(out.size());
        error_code ec = c.write(Flush::sync);
        BOOST_TEST(!ec);
        c.next_in(nullptr);
        c.avail_in(0);
        ec = c.write(Flush::finish);
        BOOST_TEST(ec == error::end_of_stream);
        c.next_in(s.data());
        c.avail_in(s.size());
        c.next_out(&out.front());
        c.avail_out(out.size());
        ec = c.write(Flush::sync);
        BOOST_TEST(ec == error::stream_error);
        ec = c.write(Flush::finish);
        BOOST_TEST(ec == error::need_buffers);
    }

    static
    void testFlushPartial(ICompressor& c)
    {
        c.init();
        std::string out;
        out.resize(1024);
        string_view s = "Hello";
        c.next_in(s.data());
        c.avail_in(s.size());
        c.next_out(&out.front());
        c.avail_out(out.size());
        error_code ec;
        ec = c.write(Flush::none);
        BOOST_TEST(!ec);
        ec = c.write(Flush::partial);
        BOOST_TEST(!ec);
    }

    static
    void testFlushAtLiteralBufferFull(ICompressor& c)
    {
        struct fixture
        {
            ICompressor& c;
            explicit fixture(ICompressor&c, std::size_t n, Strategy s) : c(c)
            {
                c.init(8, 15, 1, static_cast<int>(s));
                std::iota(in.begin(), in.end(), std::uint8_t{0});
                out.resize(n);
                c.next_in(in.data());
                c.avail_in(in.size());
                c.next_out(&out.front());
                c.avail_out(out.size());
            }

            std::array<std::uint8_t, 255> in;
            std::string out;
        };

        for (auto s : {Strategy::huffman, Strategy::rle, Strategy::normal})
        {
            {
                fixture f{c, 264, s};
                error_code ec = c.write(Flush::finish);
                BOOST_TEST(ec == error::end_of_stream);
                BOOST_TEST(c.avail_out() == 1);
            }

            {
                fixture f{c,263, s};
                error_code ec = c.write(Flush::finish);
                BOOST_TEST(!ec);
                BOOST_TEST(c.avail_out() == 0);
            }

            {
                fixture f{c, 20, s};
                error_code ec = c.write(Flush::sync);
                BOOST_TEST(!ec);
            }

        }
    }

    static
    void testRLEMatchLengthExceedLookahead(ICompressor& c)
    {
        std::vector<std::uint8_t> in;
        in.resize(300);

        c.init(8, 15, 1, static_cast<int>(Strategy::rle));
        std::fill_n(in.begin(), 4, 'a');
        std::string out;
        out.resize(in.size() * 2);
        c.next_in(in.data());
        c.avail_in(in.size());
        c.next_out(&out.front());
        c.avail_out(out.size());

        error_code ec;
        ec = c.write(Flush::sync);
        BOOST_TEST(!ec);
    }

    static
    void testFlushAfterDistMatch(ICompressor& c)
    {
        for (auto out_size : {144, 129})
        {
            std::array<std::uint8_t, 256> in{};
            // 125 will mostly fill the lit buffer, so emitting a distance code
            // results in a flush.
            auto constexpr n = 125;
            std::iota(in.begin(), in.begin() + n,
                static_cast<std::uint8_t>(0));
            std::iota(in.begin() + n, in.end(),
                static_cast<std::uint8_t>(0));

            c.init(8, 15, 1, static_cast<int>(Strategy::normal));
            std::string out;
            out.resize(out_size);
            c.next_in(in.data());
            c.avail_in(in.size());
            c.next_out(&out.front());
            c.avail_out(out.size());

            error_code ec;
            ec = c.write(Flush::sync);
            BOOST_TEST(!ec);
        }
    }

    static void testWrappedStream(){
        std::string raw = "This is fake content";
        auto test = [&](Wrap wrap){
            std::string compr;
            compr.resize(raw.size());
            auto ds = deflate_stream();
            ds.reset(6, 15, 8, Strategy::normal, wrap);
            z_params zp{};
            zp.next_in = raw.c_str();
            zp.avail_in = raw.size();
            zp.next_out = &compr[0];
            zp.avail_out = compr.size();

            error_code ec;
            ds.write(zp, Flush::full, ec);
            BOOST_TEST(!ec);
            BOOST_TEST(decompress(compr) == raw);
        };
        test(Wrap::none);
        test(Wrap::zlib);
        test(Wrap::gzip);
    }

    void
    run()
    {
        std::cerr <<
            "sizeof(deflate_stream) == " <<
            sizeof(deflate_stream) << std::endl;

        testDeflate(zlib_compressor);
        testDeflate(beast_compressor);
        testInvalidSettings(zlib_compressor);
        testInvalidSettings(beast_compressor);
        testWriteAfterFinish(zlib_compressor);
        testWriteAfterFinish(beast_compressor);
        testFlushPartial(zlib_compressor);
        testFlushPartial(beast_compressor);
        testFlushAtLiteralBufferFull(zlib_compressor);
        testFlushAtLiteralBufferFull(beast_compressor);
        testRLEMatchLengthExceedLookahead(zlib_compressor);
        testRLEMatchLengthExceedLookahead(beast_compressor);
        testFlushAfterDistMatch(zlib_compressor);
        testFlushAfterDistMatch(beast_compressor);
        testWrappedStream();
    }
};

TEST_SUITE(deflate_stream_test, "deflate_stream");

} // deflate
} // boost
