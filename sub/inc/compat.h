/* Copyright [2017] LE GARREC Vincent
 *
 * This file is part of 2LGC.
 *
 * 2LGC is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * 2LGC is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with 2LGC. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMPAT_H_
#define COMPAT_H_

#ifdef _MSC_VER
#define CHK _Check_return_

#ifndef ssize_t
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif // !ssize_t

#else // !_MSC_VER
#define CHK __attribute__ ((__warn_unused_result__))
#endif // _MSC_VER

#if defined(_MSC_VER)
#define SSCANF sscanf_s
#define DllExport __declspec(dllexport)
#else // !defined(_MSC_VER)
#define SSCANF sscanf
#define DllExport
#endif // defined(_MSC_VER)

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
