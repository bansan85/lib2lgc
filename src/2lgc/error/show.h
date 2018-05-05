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
 * \brief La macro est l'équivalent d'un "return Y; ..." si la condition X
 *        n'est pas vérifiée.
 * \param X : condition à vérifier,
 * \param Y : renvoie Y si la condition X n'est pas vérifiée,
 * \param MSG : Le message afficher.
 * \param ... : Message complémentaire sous une forme compatible avec fprintf.
 */

#define BUGPARAM(PARAM, X, Y) \
  BUG(X, Y, "Programmation error.", #PARAM " = %.\n", PARAM)
/**
 * \def BUGPARAM(PARAM, TYPE, X, Y, MANAGER)
 * \brief Macro spécialement conçu pour la vérification des paramètres.
 * \param PARAM : paramètre (variable) invalide,
 * \param TYPE : type du paramètre sous forme de texte ("%s", "%d", …),
 * \param X : condition à vérifier,
 * \param Y : renvoie Y si la condition X n'est pas vérifiée,
 * \param MANAGER : Le gestionnaire d'annulation. Peut être NULL,
 */

#define BUGLIB(X, Y, LIB) BUG(X, Y, "Library error.\n", "%\n", LIB)

#define BUGCRIT(X, Y, ...) BUG(X, Y, "Critical error.\n", __VA_ARGS__)
/**
 * \def BUGCRIT(X, Y, ...)
 * \brief Macro that check return value of function that should never failed.
 * \param X : condition,
 * \param Y : return value if condition is false,
 */

#define BUGUSER(X, Y, ...) BUG(X, Y, "User error.\n", __VA_ARGS__)

#define BUGPROG(X, Y, ...) BUG(X, Y, "Programmation error.\n", __VA_ARGS__)

#define BUGCONT(X, Y) BUG(X, Y, "Throw error.\n", "")

#endif  // ERROR_SHOW_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
