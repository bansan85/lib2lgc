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
 * @file compare_decimal.cc
 * @brief Compare diff of two decimal number base on their int representation.
 * Base on idea from
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/.
 */

#include "compare_decimal.h"

// C++ system
// For int std::abs.
#include <cstdlib>
// Pour float std::abs.
#include <cmath>
#include <cstdint>

namespace math {

/**
 * @brief Internal union to convert float to number (4 bytes).
 *
 * Help to count diff ULP between two float.
 */
union Float_t {
  /**
   * @brief Constructor to initialize the default float.
   *
   * @param num The number. By default 0.0f.
   */
  Float_t(float num = 0.0f) : f(num) {}

  /**
   * @brief Is the float number negative ?
   *
   * @return true if float is negative.
   */
  bool Negative() const { return i < 0; }

  /**
   * @brief The float number.
   */
  float f;

  /**
   * @brief The int representation of the float number.
   */
  int32_t i;
};

/**
 * @brief Internal union to convert double to number (8 bytes).
 *
 * Help to count diff ULP between two double.
 */
union Double_t {
  /**
   * @brief Constructor to initialize the default double.
   *
   * @param num The number. By default 0.0.
   */
  Double_t(double num = 0.0) : d(num) {}

  /**
   * @brief Is the double number negative ?
   *
   * @return true if double is negative.
   */
  bool Negative() const { return i < 0; }

  /**
   * @brief The double number.
   */
  double d;

  /**
   * @brief The int representation of the double number.
   */
  int64_t i;
};

}  // namespace math

/**
 * @brief Function that compare equality of two decimal number that are not
 *        close to zero.
 * Internal function. Use AlmostEqualUlpsAndAbsF or AlmostEqualUlpsAndAbsD
 * instead.
 *
 * @tparam T float or double.
 * @tparam U Float_t or Double_t.
 * @tparam V int32_t or int64_t.
 * @param A Number to compare from.
 * @param B Number to compare with.
 * @param maxDiff If diff between A and B is less than maxDiff, both numbers are
 *                equals.
 * @param maxUlpsDiff If diff between int representation of A and B is less than
 *                    maxUlpsDiff, both number are equals.
 *
 * @return true is both numbers are equals.
 */
template <typename T, typename U, typename V>
static bool AlmostEqualUlpsAndAbsInternal(T A, T B, T maxDiff,
                                          int maxUlpsDiff) {
  // Check if the numbers are really close -- needed
  // when comparing numbers near zero.
  T absDiff = std::abs(A - B);
  if (absDiff <= maxDiff) {
    return true;
  }

  U uA(A);
  U uB(B);

  // Different signs means they do not match.
  if (uA.Negative() != uB.Negative()) {
    return false;
  }

  // Find the difference in ULPs.
  V ulpsDiff = std::abs(uA.i - uB.i);
  if (ulpsDiff <= maxUlpsDiff) {
    return true;
  }

  return false;
}

/**
 * @brief Function that compare equality of two decimal number that are close to
 * zero.
 *
 * @tparam T float or double.
 * @param A Number to compare from.
 * @param B Number to compare with.
 * @param maxDiff If diff between A and B is less than maxDiff, both numbers are
 *                equals.
 * @param maxRelDiff If diff between the two numbers is smaller than maxRelDiff
 *                   * the max of the two numbers, both number are equals.
 *
 * @return true is both numbers are equals.
 */
template <typename T>
static bool AlmostEqualRelativeAndAbsInternal(T A, T B, T maxDiff,
                                              T maxRelDiff) {
  // Check if the numbers are really close -- needed
  // when comparing numbers near zero.
  T diff = std::abs(A - B);
  if (diff <= maxDiff) {
    return true;
  }

  A = std::abs(A);
  B = std::abs(B);
  T largest = (B > A) ? B : A;

  if (diff <= largest * maxRelDiff) {
    return true;
  }
  return false;
}

/**
 * @brief Compare two float numbers that are not close to zero.
 *
 * @param A Number to compare from.
 * @param B Number to compare with.
 * @param maxDiff If diff between A and B is less than maxDiff, both numbers are
 *                equals.
 * @param maxUlpsDiff If diff between int representation of A and B is less than
 *                    maxUlpsDiff, both number are equals.
 *
 * @return true is both numbers are equals.
 */
bool math::AlmostEqualUlpsAndAbsF(float A, float B, float maxDiff,
                                  int maxUlpsDiff) {
  return AlmostEqualUlpsAndAbsInternal<float, Float_t, int32_t>(A, B, maxDiff,
                                                                maxUlpsDiff);
}

/**
 * @brief Compare two float numbers that are close to zero.
 *
 * @param A Number to compare from.
 * @param B Number to compare with.
 * @param maxDiff If diff between A and B is less than maxDiff, both numbers are
 *                equals.
 * @param maxRelDiff If diff between the two numbers is smaller than maxRelDiff
 *                   * the max of the two numbers, both number are equals.
 *
 * @return true is both numbers are equals.
 */
bool math::AlmostEqualRelativeAndAbsF(float A, float B, float maxDiff,
                                      float maxRelDiff) {
  return AlmostEqualRelativeAndAbsInternal<float>(A, B, maxDiff, maxRelDiff);
}

/**
 * @brief Compare two double numbers that are not close to zero.
 *
 * @param A Number to compare from.
 * @param B Number to compare with.
 * @param maxDiff If diff between A and B is less than maxDiff, both numbers are
 *                equals.
 * @param maxUlpsDiff If diff between int representation of A and B is less than
 *                    maxUlpsDiff, both number are equals.
 *
 * @return true is both numbers are equals.
 */
bool math::AlmostEqualUlpsAndAbsD(double A, double B, double maxDiff,
                                  int maxUlpsDiff) {
  return AlmostEqualUlpsAndAbsInternal<double, Double_t, int64_t>(A, B, maxDiff,
                                                                  maxUlpsDiff);
}

/**
 * @brief Compare two double numbers that are close to zero.
 *
 * @param A Number to compare from.
 * @param B Number to compare with.
 * @param maxDiff If diff between A and B is less than maxDiff, both numbers are
 *                equals.
 * @param maxRelDiff If diff between the two numbers is smaller than maxRelDiff
 *                   * the max of the two numbers, both number are equals.
 *
 * @return true is both numbers are equals.
 */
bool math::AlmostEqualRelativeAndAbsD(double A, double B, double maxDiff,
                                      double maxRelDiff) {
  return AlmostEqualRelativeAndAbsInternal<double>(A, B, maxDiff, maxRelDiff);
}
