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

## Compilation

`-DCMAKE_CXX_FLAGS=...` custom flags.

`CC=clang CXX=clang++ cmake -DCMAKE_EXE_LINKER_FLAGS="-lgcc_s"` for clang support in gcc environment.

## Quality

`-DWALL=1` Add lots of warnings.

`-DCLANGFORMAT=1` Enable clang-format.

`-DCLANG_FORMAT=...` path to clang-format program.

`-DIWYU=1` Enable include-what-you-use.

`-Diwyu_path=...` path to iwyu folder.

`-DCOVERAGE=1` Enable coverage.

`-DCPP_LINT=1` Enable cpplint.

`-DCPPLINT=...` path to cpplint.py program.

`-DCLANGTIDY=1` Enable clang-tidy.

`-DCLANG_TIDY=...` path to clang-tidy program.

`-DPMD=1` Enable pmd.

`-DPMD_BIN=...` path to run.sh program.

`-DSWIG=...` path to swig program.

## Behavious

`-DDISABLE_VISITABLE_CACHE=1` disable cache in visitor pattern.

`-DGRPC_LIBRARY=...` path to GRPC library. If you want to add support for GRPC publisher.

`-DTEMPLATE_CLASS=1` Don't use source in header file. So you will need to explicitly add symbol with `template class ...`.

# Dependencies

## Protobuf
All messages are send by using protobuf message.

## OpenSSL
Actually not needed but will be for https publisher.

# TODO
Missing local README.md means that the functionnality is still under development.

  - pattern command / undomanager.
