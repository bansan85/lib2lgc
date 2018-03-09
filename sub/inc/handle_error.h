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

#ifndef HANDLE_ERROR_H_
#define HANDLE_ERROR_H_

#include <cstdio>

#define PRINTF(...) printf(__VA_ARGS__)

// BUG: for internal use only.
#define BUG(X, Y, MSG, ...)                                                   \
  do                                                                          \
  {                                                                           \
    if (!(X))                                                                 \
    {                                                                         \
      PRINTF("file %s, function %s, line %d, text: ", __FILE__, __FUNCTION__, \
             __LINE__);                                                       \
      PRINTF(MSG);                                                            \
      PRINTF(__VA_ARGS__);                                                    \
      return Y;                                                               \
    }                                                                         \
  } while (0)
/**
 * \def BUG(X, Y, MANAGER, MSG, ...)
 * \brief La macro est l'équivalent d'un "return Y; ..." si la condition X
 *        n'est pas vérifiée.
 * \param X : condition à vérifier,
 * \param Y : renvoie Y si la condition X n'est pas vérifiée,
 * \param MANAGER : Le gestionnaire d'annulation. Peut être NULL,
 * \param MSG : Le message afficher.
 * \param ... : Message complémentaire sous une forme compatible avec fprintf.
 */

#define BUGPARAM(PARAM, TYPE, X, Y)                                   \
  BUG(X, Y, "Erreur de programmation.", "%s" #PARAM " = " TYPE ".\n", \
      "Paramètre incorrect: ", PARAM)
/**
 * \def BUGPARAM(PARAM, TYPE, X, Y, MANAGER)
 * \brief Macro spécialement conçu pour la vériifcation des paramètres.
 * \param PARAM : paramètre (variable) invalide,
 * \param TYPE : type du paramètre sous forme de texte ("%s", "%d", …),
 * \param X : condition à vérifier,
 * \param Y : renvoie Y si la condition X n'est pas vérifiée,
 * \param MANAGER : Le gestionnaire d'annulation. Peut être NULL,
 */

#define BUGLIB(X, Y, LIB) \
  BUG(X, Y, "Erreur depuis la librairie.\n", "%s\n", LIB)

#define BUGCRIT(X, Y, ...) BUG(X, Y, "Erreur de type critique.\n", __VA_ARGS__)
/**
 * \def BUGCRIT(X, Y, ...)
 * \brief Cette macro est identique à la commande #BUG mais ajoute un message
 *        d'erreur "Erreur critique" avant l'arrêt de la fonction.
 * \param X : condition à vérifier,
 * \param Y : renvoie Y si la condition X n'est pas vérifiée,
 * \param MANAGER : Le gestionnaire d'annulation. Peut être NULL,
 */

#define BUGUSER(X, Y, ...) \
  BUG(X, Y, "Erreur de type utilisateur.\n", __VA_ARGS__)

#define BUGPROG(X, Y, ...) BUG(X, Y, "Erreur de programmation.\n", __VA_ARGS__)

#define BUGCONT(X, Y) BUG(X, Y, "Propagation de l'erreur.\n", "BLA")

#endif  // HANDLE_ERROR_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
