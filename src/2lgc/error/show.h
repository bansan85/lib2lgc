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

#ifndef ERROR_SHOW_H_
#define ERROR_SHOW_H_

#include <iostream>
#include <string>

#define BUG(OUT, X, Y, MSG, Z)                                                 \
  do                                                                           \
  {                                                                            \
    if (!(X))                                                                  \
    {                                                                          \
      (OUT) << "file " << __FILE__ << ", function "                            \
            << static_cast<const char*>(__FUNCTION__) << ", line " << __LINE__ \
            << ", type: " << (MSG) << (Z);                                     \
      return Y;                                                                \
    }                                                                          \
  } while (0)

#define BUGPARAM(OUT, PARAM, X, Y) \
  BUG(OUT, X, Y,                   \
      "Programmation error: ", #PARAM " = " + std::to_string(PARAM) + "%.\n")

#define BUGLIB(OUT, X, Y, LIB) \
  BUG(OUT, X, Y, "Library error: ", (LIB) + std::string("\n"))

#define BUGCRIT(OUT, X, Y, Z) BUG(OUT, X, Y, "Critical error.\n", Z)

#define BUGUSER(OUT, X, Y, Z) BUG(OUT, X, Y, "User error.\n", Z)

#define BUGPROG(OUT, X, Y, Z) BUG(OUT, X, Y, "Programmation error.\n", Z)

#define BUGCONT(OUT, X, Y) BUG(OUT, X, Y, "Throw error.\n", "")

#endif  // ERROR_SHOW_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
