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

#ifndef HANDLE_ERROR_H_
#define HANDLE_ERROR_H_

#include <stdio.h>

#define PRINTF(...) printf (__VA_ARGS__)

// BUG: for internal use only.
#define BUG(X, Y, MSG, ...) \
  do { \
    if (!(X)) { \
      PRINTF ("file %s, function %s, line %d, text: ", \
              __FILE__, \
              __FUNCTION__, \
              __LINE__); \
      PRINTF (MSG); \
      PRINTF (__VA_ARGS__); \
      return Y; \
    } \
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

#define BUGPARAM(PARAM, TYPE, X, Y) \
  BUG (X, Y, \
       "Erreur de programmation.", \
       "%s" #PARAM " = " TYPE ".\n", "Paramètre incorrect: ", PARAM)
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
  BUG (X, Y, "Erreur depuis la librairie.\n", "%s\n", LIB)

#define BUGCRIT(X, Y, ...) \
  BUG (X, Y, "Erreur de type critique.\n", __VA_ARGS__)
/**
 * \def BUGCRIT(X, Y, ...)
 * \brief Cette macro est identique à la commande #BUG mais ajoute un message
 *        d'erreur "Erreur critique" avant l'arrêt de la fonction.
 * \param X : condition à vérifier,
 * \param Y : renvoie Y si la condition X n'est pas vérifiée,
 * \param MANAGER : Le gestionnaire d'annulation. Peut être NULL,
 */

#define BUGUSER(X, Y, ...) \
  BUG (X, Y, "Erreur de type utilisateur.\n", __VA_ARGS__)

#define BUGPROG(X, Y, ...) \
  BUG (X, Y, "Erreur de programmation.\n", __VA_ARGS__)

#define BUGCONT(X, Y) \
  BUG (X, Y, "Propagation de l'erreur.\n", "BLA")

#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
