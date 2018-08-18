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

// BUG: for internal use only.
#define BUG(OUT, X, Y, MSG, Z)                                               \
  do                                                                         \
  {                                                                          \
    if (!(X))                                                                \
    {                                                                        \
      OUT << "file " << __FILE__ << ", function "                            \
          << static_cast<const char*>(__FUNCTION__) << ", line " << __LINE__ \
          << ", type: " << MSG << Z;                                         \
      return Y;                                                              \
    }                                                                        \
  } while (0)
/**
 * \def BUG(OUT, X, Y, MSG, Z)
 * \brief If condition X is false, show messages then return Y.
 * \param OUT Output stream.
 * \param X Condition to check.
 * \param Y Return value if X is wrong.
 * \param MSG First message to show.
 * \param Z Additionnal message.
 */

#define BUGPARAM(OUT, PARAM, X, Y) \
  BUG(OUT, X, Y, "Programmation error.", #PARAM " = " << PARAM << "%.\n")
/**
 * \def BUGPARAM(OUT, PARAM, X, Y)
 * \brief Macro that tell that a parameter is invalid.
 * \param OUT Output stream.
 * \param PARAM The invalid parameter. Just the variable. It will be used to
 * show the name and the value.
 * \param X Condition to check. This condition is supposed to be always true.
 * If in normal use, this condition may be wrong, use BUGUSER.
 * \param Y Return value if X is wrong.
 */

#define BUGLIB(OUT, X, Y, LIB) BUG(OUT, X, Y, "Library error: ", LIB << "\n")
/**
 * \def BUGLIB(OUT, X, Y, LIB)
 * \brief If condition X is false, tell which library then return Y.
 * \param OUT Output stream.
 * \param X Condition to check. This condition is supposed to be always true.
 * If the condition are wrong, the error comes from a library and the error is
 * because of the library.
 * If in normal use, the condition may be wrong, use BUGUSER.
 * \param Y Return value if X is wrong.
 * \param LIB Name of the library. You can't had additionnal message. If a
 * message is needed, it means that you need to use BUGUSER or BUGCRIT.
 */

#define BUGCRIT(OUT, X, Y, Z) BUG(OUT, X, Y, "Critical error.\n", Z)
/**
 * \def BUGCRIT(OUT, X, Y, Z)
 * \brief Macro that check return value of function that should never failed.
 * If the condition are wrong, it a unknown error.
 * \param OUT Output stream.
 * \param X Condition to check. This condition is supposed to be always true.
 * If in normal use, this condition may be wrong, use BUGUSER.
 * \param Y Return value if condition is false,
 * \param Z Additionnal message.
 */

#define BUGUSER(OUT, X, Y, Z) BUG(OUT, X, Y, "User error.\n", Z)
/**
 * \def BUGUSER(OUT, X, Y, Z)
 * \brief Macro that check return value of function that may fail.
 * If the condition are wrong, it's user fault.
 * \param OUT Output stream.
 * \param X Condition to check.
 * \param Y Return value if condition is false,
 * \param Z Additionnal message.
 */

#define BUGPROG(OUT, X, Y, Z) BUG(OUT, X, Y, "Programmation error.\n", Z)
/**
 * \def BUGPROG(OUT, X, Y, Z)
 * \brief Macro that check return value of function that should never failed.
 * If the condition are wrong, it a programmation error (not implemented switch
 * for example).
 * \param OUT Output stream.
 * \param X Condition to check. This condition is supposed to be always true.
 * If in normal use, this condition may be wrong, use BUGUSER.
 * \param Y Return value if condition is false,
 * \param Z Additionnal message.
 */

#define BUGCONT(OUT, X, Y) BUG(OUT, X, Y, "Throw error.\n", "")
/**
 * \def BUGCONT(OUT, X, Y, ...)
 * \brief Macro that check return value of function where return value is
 * covered with BUG*. So you don't need to write something, the BUG* should have
 * done the work.
 * \param OUT Output stream.
 * \param X Condition to check.
 * \param Y Return value if condition is false,
 */

#endif  // ERROR_SHOW_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
