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

/**
 * @file compare_decimal.h
 * @brief Compare diff of two decimal number base on their int representation.
 * Base on idea from
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/.
 */

#ifndef MATH_COMPARE_DECIMAL_H_
#define MATH_COMPARE_DECIMAL_H_

#include <2lgc/compat.h>

namespace llgc::math
{
/**
 * @brief Class that extends math functionnality.
 */
class Compare
{
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

}  // namespace llgc::math

#endif  // MATH_COMPARE_DECIMAL_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
