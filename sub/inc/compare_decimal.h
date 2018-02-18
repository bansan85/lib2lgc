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

/**
 * @file compare_decimal.h
 * @brief Compare diff of two decimal number base on their int representation.
 * Base on idea from
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/.
 */

#ifndef COMPARE_DECIMAL_H_
#define COMPARE_DECIMAL_H_

#include <compat.h>

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
