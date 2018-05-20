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

### Compilation

`-DCMAKE_CXX_FLAGS=...` custom flags.

`CC=clang CXX=clang++ cmake -DCMAKE_EXE_LINKER_FLAGS="-lgcc_s"` for clang support in gcc environment.

`-DCHECK_CODE=1` Add optimisations and warnings and run a lot of static analyser: iwyu, coverage with lcov and genhtml, clang-format, cpplint, cppcheck, clang-tidy with run-clang-tidy.py, doxygen, nsiqcppstyle, pmd, coverity.

`-Diwyu_path=...` path to iwyu folder. Optional for `make check`.

`-DCLANG_FORMAT=...` path to clang-format program. Optional for `make check`.

`-DCPPLINT=...` path to cpplint.py program. Optional for `make check`.

`-DCLANG_TIDY=...` path to clang-tidy program. Optional for `make check`.

`-DNSIQCPPSTYLE=...` path to nsiqcppstyle program. Optional for `make check`.

`-DPMD_BIN=...` path to run.sh program. Optional for `make check`.

`-DSWIG=...` path to swig program. Optional for `make check`.

### Behavious

`-DDISABLE_VISITABLE_CACHE=1` disable cache in visitor pattern.

# Dependencies

## Protobuf
All messages are send by using protobuf message.

## OpenSSL
Actually not needed but will be for https publisher.

# TODO
Missing local README.md means that the functionnality is still under development.

  - command pattern / undomanager.
