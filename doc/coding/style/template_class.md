# Rules to write template class

# Rules

The goal is to define rules to move source outside of header for template classes.

If `TEMPLATE_CLASS` is defined at compile-time, source will be outside of header. If not, header will have source as inline.

# Coding style

## Header (.h)

Add on top:
```
#include <2lgc/compat.h>
```

And add on bottom:
```
#ifndef TEMPLATE_CLASS
#include <2lgc/file.cc>
#endif
```

## Source (.cc)

Add on top:
```
#ifndef FILE_XXXX_CC_
#define FILE_XXXX_CC_

#include <2lgc/compat.h>
```

And add for each template function `INLINE_TEMPLATE` like this:
```
template <typename T, typename U>
INLINE_TEMPLATE
std::unique_ptr<U> llgc::file::XXX<T, U>::Create(
```

Add on bottom:
```
#endif  //  FILE_XXXX_CC_
```

### Application (.cc)

If you don't want your application to support `template class`, you don't have to do anything.

If you do want, do the following lines.

Add on top:
```
#include <2lgc/compat.h>
```

Then add after headers, all source file that define `template class`:
```
#ifdef TEMPLATE_CLASS
#include <2lgc/file.cc>
#endif
```

```
#ifdef TEMPLATE_CLASS
template class llgc::file::XXX<TTTT, UUUU>;
#endif
```

## CMakeLists.txt

You must add all sources that define template classes in headers no matter if `template class` is enabled or not.

Again, no matter if `template class` is enabled or not, you must not add sources that define template classes in source to compile.
