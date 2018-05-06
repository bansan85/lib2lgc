# lib2lgc

[![Travis Status](https://travis-ci.org/bansan85/lib2lgc.svg?branch=master)](https://travis-ci.org/bansan85/lib2lgc)
[![Coverity Status](https://scan.coverity.com/projects/1279/badge.svg)](https://scan.coverity.com/projects/1279)

# Compile

## Standard way

```
mkdir build
cd build
cmake ..
make
make check
```

## Options

`-DCHECK_CODE=1` Add optimisations and warnings and run a lot of static analyser: iwyu, coverage with lcov and genhtml, clang-format, cpplint, cppcheck, clang-tidy with run-clang-tidy.py, doxygen, nsiqcppstyle, pmd, coverity.

`-DDISABLE_VISITABLE_CACHE=1` disable cache in visitor pattern.

`-DCMAKE_CXX_FLAGS=...` custom flags.

# Dependencies

## Protobuf
All messages are send by using protobuf message.

## OpenSSL
If OpenSSL is found, TCP acknowledges will be SHA512 sum. If not found, djb2 will be used.

# TODO
Missing local README.md means that the functionnality is still under development.

  - command pattern,
