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

/**
 * @file compare_decimal.h
 * @brief Compare diff of two decimal number base on their int representation.
 * Base on idea from
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/.
 */

#ifndef COMPARE_DECIMAL_H_
#define COMPARE_DECIMAL_H_

#include "config.h"

/**
 * @brief Class that extends math functionnality.
 */
class Math {
 public:
  static bool AlmostEqualUlpsAndAbsF(float A, float B, float maxDiff,
                                     int maxUlpsDiff) CHK;
  static bool AlmostEqualRelativeAndAbsF(float A, float B, float maxDiff,
                                         float maxRelDiff) CHK;

  static bool AlmostEqualUlpsAndAbsD(double A, double B, double maxDiff,
                                     int maxUlpsDiff) CHK;
  static bool AlmostEqualRelativeAndAbsD(double A, double B, double maxDiff,
                                         double maxRelDiff) CHK;
};

#endif  // COMPARE_DECIMAL_H_
