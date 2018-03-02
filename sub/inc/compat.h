/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef COMPAT_H_
#define COMPAT_H_

#ifdef _MSC_VER
#define CHK _Check_return_

#ifndef ssize_t
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif  // !ssize_t

#else  // !_MSC_VER
#define CHK __attribute__((__warn_unused_result__))
#endif  // _MSC_VER

#if defined(_MSC_VER)
#define SSCANF sscanf_s
#define DllExport __declspec(dllexport)
#else  // !defined(_MSC_VER)
#define SSCANF sscanf
#define DllExport
#endif  // defined(_MSC_VER)

#endif  // COMPAT_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
