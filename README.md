# Deflate
<img width="880" height = "80" alt = "Boost.Deflate Title"
    src="https://raw.githubusercontent.com/ryanjanson/deflate/master/doc/images/repo-logo.png">

Branch          | Travis | Appveyor | Azure Pipelines | codecov.io | Docs | Matrix |
:-------------: | ------ | -------- | --------------- | ---------- | ---- | ------ |
[`master`](https://github.com/ryanjanson/deflate/tree/master) | [![Build Status](https://travis-ci.com/ryanjanson/deflate.svg?branch=master)](https://travis-ci.com/ryanjanson/deflate) | [![Build status](https://ci.appveyor.com/api/projects/status/github/ryanjanson/deflate?branch=master&svg=true)](https://ci.appveyor.com/project/aerostun/deflate/branch/master) | [![Build Status](https://img.shields.io/azure-devops/build/ryandjanson/83af1fcc-e2cb-4fb5-8f07-03771708f44a/1/master)](https://ryandjanson.visualstudio.com/Boost.Deflate/_build/latest?definitionId=1&branchName=master) | [![codecov](https://codecov.io/gh/ryanjanson/deflate/branch/master/graph/badge.svg)](https://codecov.io/gh/ryanjanson/deflate/branch/master) | [![Documentation](https://img.shields.io/badge/docs-master-brightgreen.svg)](http://aerostun.github.com/doc/deflate/index.html) | [![Matrix](https://img.shields.io/badge/matrix-master-brightgreen.svg)](http://www.boost.org/development/tests/master/developer/deflate.html)
[`develop`](https://github.com/ryanjanson/deflate/tree/develop) | [![Build Status](https://travis-ci.com/ryanjanson/deflate.svg?branch=develop)](https://travis-ci.com/ryanjanson/deflate) | [![Build status](https://ci.appveyor.com/api/projects/status/github/ryanjanson/deflate?branch=develop&svg=true)](https://ci.appveyor.com/project/aerosrun/deflate/branch/develop) | [![Build Status](https://img.shields.io/azure-devops/build/ryandjanson/83af1fcc-e2cb-4fb5-8f07-03771708f44a/1/develop)](https://ryandjanson.visualstudio.com/Boost.Deflate/_build/latest?definitionId=1&branchName=develop) | [![codecov](https://codecov.io/gh/ryanjanson/deflate/branch/develop/graph/badge.svg)](https://codecov.io/gh/ryanjanson/deflate/branch/develop) | [![Documentation](https://img.shields.io/badge/docs-develop-brightgreen.svg)](http://aerostun.github.com/doc/deflate/index.html) | [![Matrix](https://img.shields.io/badge/matrix-develop-brightgreen.svg)](http://www.boost.org/development/tests/develop/developer/deflate.html)

# Boost.Deflate

## This is currently **NOT** an official Boost library.

## Introduction

This library provides a C++11 (and above) implementation of the Deflate compression format (described in RFC 1951) as
well as the zlib (RFC1950) and gzip (RFC1952) data formats. Its design is currently based on [zlib](https://zlib.net)
and is mainly composed of original work by Vinnie Falco based on the zlib library.

## Motivation

A quick search on compression algorithms reveals a myriad of different specifications and implementations. One of the
most notable is the Deflate format which is used in a lot of other file formats (PNG, ZIP, PDF, ...) as well as in
data transfers over HTTP and WebSockets. While the zlib library does a great job of providing a good reference C
implementation, it lacks a proper modern C++ interface making it harder to use and possibly unsuitable for pure C++ code
or even other Boost projects, which ["**should not** use libraries other than Boost or the C++ Standard Library"](https://www.boost.org/development/reuse.html).
This library could thus be used to bridge the gap between common data formats and the Boost environment without having
each project rolling its own version.  

## Design Goals

This library is not a [zlib](https://zlib.net) wrapper, however since zlib is industry standard software and has been so
for now decades, it should be able to accomplish the same tasks with comparable performance and allow zlib users to
switch to this library without great trouble.

The design of the library also achieves these goals:
- Requires only C++11 when used with Boost
- Fast compilation performance
- Uniform interface across all supported C++ versions
- Be usable in a standalone mode (without Boost) in C++17 and above

## License

Distributed under the Boost Software License, Version 1.0.
(See accompanying file [LICENSE_1_0.txt](LICENSE_1_0.txt) or copy at
https://www.boost.org/LICENSE_1_0.txt)
