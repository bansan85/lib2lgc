<h1>lib2lgc</h1>
[![Travis Status](https://travis-ci.org/bansan85/lib2lgc.svg?branch=master)](https://travis-ci.org/bansan85/lib2lgc)
[![Coverity Status](https://scan.coverity.com/projects/1279/badge.svg)](https://scan.coverity.com/projects/1279)

<h1>Compile</h1>
<h2>Standard way</h2>

```
mkdir build
cd build
cmake ..
make
make check
```

<h2>Options</h2>
`-DCHECK_CODE=1` Add optimisations and warnings and run a lot of static analyser: iwyu, coverage with lcov and genhtml, clang-format, cpplint, cppcheck, clang-tidy with run-clang-tidy.py, doxygen, nsiqcppstyle, pmd, coverity.

`-DDISABLE_VISITABLE_CACHE=1` disable cache in visitor pattern.

`-DCMAKE_CXX_FLAGS=...` custom flags.
