/*
 *    Copyright 2018 LE GARREC Vincent
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COMPAT_H_
#define COMPAT_H_

#include <2lgc/config.h>

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

#ifdef NO_CPP17_NAMESPACE
#define START_NAMESPACE2(X, Y) namespace X { namespace Y {
#define END_NAMESPACE2(X, Y) } }
#define START_NAMESPACE3(X, Y, Z) namespace X { namespace Y { namespace Z {
#define END_NAMESPACE3(X, Y, Z) } } }
#else
#define START_NAMESPACE2(X, Y) namespace X::Y {
#define END_NAMESPACE2(X, Y) }
#define START_NAMESPACE3(X, Y, Z) namespace X::Y::Z {
#define END_NAMESPACE3(X, Y, Z) }
#endif

#endif  // COMPAT_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
