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

#include <2lgc/override/printf.h>
#include <iostream>

// BUG: for internal use only.
#define BUG(X, Y, MSG, ...)                                          \
  do                                                                 \
  {                                                                  \
    if (!(X))                                                        \
    {                                                                \
      llgc::override::Print::F(                                      \
          std::cout, "file %, function %, line %, type: ", __FILE__, \
          static_cast<const char*>(__FUNCTION__), __LINE__);         \
      llgc::override::Print::F(std::cout, MSG);                      \
      llgc::override::Print::F(std::cout, __VA_ARGS__);              \
      return Y;                                                      \
    }                                                                \
  } while (0)
/**
 * \def BUG(X, Y, MSG, ...)
 * \brief If condition X is false, show messages then return Y.
 * \param X Condition to check.
 * \param Y Return value if X is wrong.
 * \param MSG First message to show.
 * \param ... Additionnal message.
 */

#define BUGPARAM(PARAM, X, Y) \
  BUG(X, Y, "Programmation error.", #PARAM " = %.\n", PARAM)
/**
 * \def BUGPARAM(PARAM, X, Y)
 * \brief Macro that tell that a parameter is invalid.
 * \param PARAM The invalid parameter. Just the variable. It will be used to
 * show the name and the value.
 * \param X Condition to check. This condition is supposed to be always true.
 * If in normal use, this condition may be wrong, use BUGUSER.
 * \param Y Return value if X is wrong.
 */

#define BUGLIB(X, Y, LIB) BUG(X, Y, "Library error.\n", "%\n", LIB)
/**
 * \def BUGLIB(X, Y, LIB)
 * \brief If condition X is false, tell which library then return Y.
 * \param X Condition to check. This condition is supposed to be always true.
 * If the condition are wrong, the error comes from a library and the error is
 * because of the library.
 * If in normal use, the condition may be wrong, use BUGUSER.
 * \param Y Return value if X is wrong.
 * \param LIB Name of the library. You can't had additionnal message. If a
 * message is needed, it means that you need to use BUGUSER or BUGCRIT.
 */

#define BUGCRIT(X, Y, ...) BUG(X, Y, "Critical error.\n", __VA_ARGS__)
/**
 * \def BUGCRIT(X, Y, ...)
 * \brief Macro that check return value of function that should never failed.
 * If the condition are wrong, it a unknown error.
 * \param X Condition to check. This condition is supposed to be always true.
 * If in normal use, this condition may be wrong, use BUGUSER.
 * \param Y Return value if condition is false,
 * \param ... Additionnal message.
 */

#define BUGUSER(X, Y, ...) BUG(X, Y, "User error.\n", __VA_ARGS__)
/**
 * \def BUGUSER(X, Y, ...)
 * \brief Macro that check return value of function that may fail.
 * If the condition are wrong, it's user fault.
 * \param X Condition to check.
 * \param Y Return value if condition is false,
 * \param ... Additionnal message.
 */

#define BUGPROG(X, Y, ...) BUG(X, Y, "Programmation error.\n", __VA_ARGS__)
/**
 * \def BUGPROG(X, Y, ...)
 * \brief Macro that check return value of function that should never failed.
 * If the condition are wrong, it a programmation error (not implemented switch
 * for example).
 * \param X Condition to check. This condition is supposed to be always true.
 * If in normal use, this condition may be wrong, use BUGUSER.
 * \param Y Return value if condition is false,
 * \param ... Additionnal message.
 */

#define BUGCONT(X, Y) BUG(X, Y, "Throw error.\n", "")
/**
 * \def BUGCONT(X, Y, ...)
 * \brief Macro that check return value of function where return value is
 * covered with BUG*. So you don't need to write something, the BUG* should have
 * done the work.
 * \param X Condition to check.
 * \param Y Return value if condition is false,
 */

#endif  // ERROR_SHOW_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
