//
// Copyright (c) 2020 Ryan Janson (ryand.janson@gmail.com)
// Copyright (c) 2019-2020 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/ryanjanson/deflate
//

#ifndef BOOST_DEFLATE_DETAIL_CONFIG_HPP
#define BOOST_DEFLATE_DETAIL_CONFIG_HPP

#ifndef BOOST_DEFLATE_STANDALONE
# include <boost/config.hpp>
# include <boost/assert.hpp>
# include <boost/system/error_code.hpp>
# include <boost/system/system_error.hpp>
# include <boost/utility/string_view.hpp>
#else
# include <cassert>
# include <string_view>
# include <system_error>
#endif
#include <stdint.h>

// detect 32/64 bit
#if UINTPTR_MAX == UINT64_MAX
# define BOOST_DEFLATE_ARCH 64
#elif UINTPTR_MAX == UINT32_MAX
# define BOOST_DEFLATE_ARCH 32
#else
# error Unknown or unsupported architecture, please open an issue
#endif

#if defined(__APPLE__) && defined(__clang__)
# define BOOST_DEFLATE_APPLE_DISABLES
#endif

// VFALCO Copied from <boost/config.hpp>
//        This is a derivative work.
#ifdef __has_cpp_attribute
// clang-6 accepts [[nodiscard]] with -std=c++14, but warns about it -pedantic
# if __has_cpp_attribute(nodiscard) && !(defined(__clang__) && (__cplusplus < 201703L))
#  define BOOST_DEFLATE_NODISCARD [[nodiscard]]
# else
#  define BOOST_DEFLATE_NODISCARD
# endif
#else
# define BOOST_DEFLATE_NODISCARD
#endif

#ifndef BOOST_DEFLATE_FORCEINLINE
# ifdef _MSC_VER
#  define BOOST_DEFLATE_FORCEINLINE __forceinline
# else
#  define BOOST_DEFLATE_FORCEINLINE inline
# endif
#endif

#ifndef BOOST_NO_EXCEPTIONS
# define BOOST_DEFLATE_THROW(x) throw(x)
#else
# define BOOST_DEFLATE_THROW(x) do{}while(0)
#endif

#ifndef BOOST_DEFLATE_STANDALONE
# define BOOST_DEFLATE_ASSERT BOOST_ASSERT
#else
# define BOOST_DEFLATE_ASSERT assert
#endif

#define BOOST_DEFLATE_STATIC_ASSERT( ... ) static_assert(__VA_ARGS__, #__VA_ARGS__)

#ifndef BOOST_DEFLATE_NO_SSE2
# if (defined(_M_IX86) && _M_IX86_FP == 2) || \
      defined(_M_X64) || defined(__SSE2__)
#  define BOOST_DEFLATE_USE_SSE2
# endif
#endif

#ifndef BOOST_DEFLATE_STANDALONE
# if defined(GENERATING_DOCUMENTATION)
#  define BOOST_DEFLATE_DECL
# elif defined(BOOST_DEFLATE_HEADER_ONLY)
#  define BOOST_DEFLATE_DECL  inline
# else
#  if (defined(BOOST_DEFLATE_DYN_LINK) || defined(BOOST_ALL_DYN_LINK)) && !defined(BOOST_DEFLATE_STATIC_LINK)
#   if defined(BOOST_DEFLATE_SOURCE)
#    define BOOST_DEFLATE_DECL  BOOST_SYMBOL_EXPORT
#    define BOOST_DEFLATE_BUILD_DLL
#   else
#    define BOOST_DEFLATE_DECL  BOOST_SYMBOL_IMPORT
#   endif
#  endif // shared lib
#  ifndef  BOOST_DEFLATE_DECL
#   define BOOST_DEFLATE_DECL
#  endif
#  if !defined(BOOST_DEFLATE_SOURCE) && !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_DEFLATE_NO_LIB)
#   define BOOST_LIB_NAME boost_uri
#   if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_DEFLATE_DYN_LINK)
#    define BOOST_DYN_LINK
#   endif
//#   include <boost/config/auto_link.hpp>
#  endif  // auto-linking disabled
# endif
#else
# ifdef BOOST_DEFLATE_HEADER_ONLY
#  define BOOST_DEFLATE_DECL  inline
# else
#  define BOOST_DEFLATE_DECL
# endif
#endif

// These macros are private, for tests, do not change
// them or else previously built libraries won't match.
#ifndef BOOST_DEFLATE_MAX_OBJECT_SIZE
# define BOOST_DEFLATE_NO_MAX_OBJECT_SIZE
# define BOOST_DEFLATE_MAX_OBJECT_SIZE  0x7ffffffe
#endif
#ifndef BOOST_DEFLATE_MAX_ARRAY_SIZE
# define BOOST_DEFLATE_NO_MAX_ARRAY_SIZE
# define BOOST_DEFLATE_MAX_ARRAY_SIZE   0x7ffffffe
#endif
#ifndef BOOST_DEFLATE_MAX_STRING_SIZE
# define BOOST_DEFLATE_NO_MAX_STRING_SIZE
# define BOOST_DEFLATE_MAX_STRING_SIZE  0x7ffffffe
#endif
#ifndef BOOST_DEFLATE_MAX_STACK_SIZE
# define BOOST_DEFLATE_NO_MAX_STACK_SIZE
# define BOOST_DEFLATE_MAX_STACK_SIZE  ((::size_t)(-1))
#endif
#ifndef BOOST_DEFLATE_PARSER_BUFFER_SIZE
# define BOOST_DEFLATE_NO_PARSER_BUFFER_SIZE
# define BOOST_DEFLATE_PARSER_BUFFER_SIZE 2048
#endif

#endif
