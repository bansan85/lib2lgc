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

## Options

`-DWALL=1` Add lots of warnings.

`-DIWYU=1` Enable include-what-you-use.

`-Diwyu_path=...` path to iwyu folder.

`-DCOVERAGE=1` Enable coverage.

`-DSWIG=...` path to swig program.

## make
`make clang-format` runs clang-format. `-DCLANG_FORMAT=...` optional path to clang-format program.

`make cppcheck` runs cppcheck. `-DCPPCHECK_BIN=...` optional path to cppcheck program. Errors like `(performance) Function parameter 'nom' should be passed by const reference.` are ignored due to conflit with clang-tidy.

`make cpplint` runs cpplint.py. `-DCPPLINT=...` optional path to cpplint.py script.

`make clang-tidy` runs clang-tidy. `-DCLANG_TIDY=...` optional path to clang-tidy program.

`make pmd` runs pmd. `-DPMD_BIN=...` optional path to pmd program.

`make doc` runs doxygen.

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
  - CI:
    - cmake ../lib2lgc -DWALL=1 -DIWYU=1 -DCOVERAGE=1 -DCMAKE_CXX_FLAGS="-DNDEBUG" && make -j9 check
    - cmake ../lib2lgc -DWALL=1 -DIWYU=1 -DTEMPLATE_CLASS=1 && make -j9 check
    - cmake ../lib2lgc -DDISABLE_VISITABLE_CACHE=1 && make -j9 check
    - CC=clang CXX=clang++ cmake ../lib2lgc -DCMAKE_EXE_LINKER_FLAGS="-lgcc_s" -DWALL && make -j9 check
    - CC=clang CXX=clang++ cmake ../lib2lgc -DCMAKE_EXE_LINKER_FLAGS="-lgcc_s" -DWALL -DTEMPLATE_CLASS=1 && make -j9 check
    - make clang-format
    - make cpplint
    - make clang-tidy
    - make pmd
    - make cppcheck
    - make check
    - scan-build cmake; scan-build make -j9 check
