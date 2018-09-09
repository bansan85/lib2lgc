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

/** \file show.h
 * \brief These functions show error when happening during execution.
 *
 *
 * \def BUG(OUT, X, Y, MSG, Z)
 * \warning For internal use only.
 * \brief If condition `X` is wrong, it's shows messages then returns `Y`.
 * \param[out] OUT Output stream.
 * \param[in] X Condition to check.
 * \param[in] Y Return value if `X` is wrong.
 * \param[in] MSG First message to show.
 * \param[in] Z Additionnal message.
 *
 *
 * \def BUGPARAM(OUT, PARAM, X, Y)
 * \brief Macro that tell that a parameter is invalid.
 * \param[out] OUT Output stream.
 * \param[in] PARAM The invalid parameter. Just the variable. It will be used to
 *       show the name and the value.
 * \param[in] X Condition to check. This condition is supposed to be always
 *       true. If in normal use, this condition may be wrong, use #BUGUSER.
 * \param[in] Y Return value if `X` is wrong.
 *
 *
 * \def BUGLIB(OUT, X, Y, LIB)
 * \brief If condition X is false, tell which library then return Y.
 * \param[out] OUT Output stream.
 * \param[in] X Condition to check. This condition is supposed to be always
 *       true. If the condition are wrong, the error comes from a library and
 *       the error is because of the library. If, in normal use, the condition
 *       may be wrong, use BUGUSER.
 * \param[in] Y Return value if X is wrong.
 * \param[in] LIB Name of the library. You can't had additionnal message. If a
 *       message is needed, it means that you need to use BUGUSER or BUGCRIT.
 *
 *
 * \def BUGCRIT(OUT, X, Y, Z)
 * \brief Macro that check return value of function that should never failed.
 *        If the condition are wrong, it a unknown error.
 * \param[out] OUT Output stream.
 * \param[in] X Condition to check. This condition is supposed to be always
 *       true. If, in normal use, this condition may be wrong, use BUGUSER.
 * \param[in] Y Return value if condition is false.
 * \param[in] Z Additionnal message.
 *
 *
 * \def BUGUSER(OUT, X, Y, Z)
 * \brief Macro that check return value of function that may fail.
 *        If the condition are wrong, it's user fault.
 * \param[out] OUT Output stream.
 * \param[in] X Condition to check.
 * \param[in] Y Return value if condition is false.
 * \param[in] Z Additionnal message.
 *
 *
 * \def BUGPROG(OUT, X, Y, Z)
 * \brief Macro that check return value of function that should never failed.
 *        If the condition are wrong, it a programmation error (not implemented
 *        switch for example).
 * \param[out] OUT Output stream.
 * \param[in] X Condition to check. This condition is supposed to be always
 *       true. If in normal use, this condition may be wrong, use BUGUSER.
 * \param[in] Y Return value if condition is false.
 * \param[in] Z Additionnal message.
 *
 *
 * \def BUGCONT(OUT, X, Y, ...)
 * \brief Macro that check return value of function where return value is
 *        covered with BUG*. So you don't need to write something, the BUG*
 *        should have done the work.
 * \param[out] OUT Output stream.
 * \param[in] X Condition to check.
 * \param[in] Y Return value if condition is false.
 *
 * \def EXECUTE_AND_ABORT(OUT, CONDITION)
 * \brief Macro that abort program if condition fails. It like an assert but
 *        execute the function even if NDEBUG is defined.
 * \param[out] OUT Output stream.
 * \param[in] CONDITION Condition to check. Can be a non-const function.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
