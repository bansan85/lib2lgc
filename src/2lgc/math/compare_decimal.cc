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
 * @file compare_decimal.cc
 * @brief Compare diff of two decimal number base on their int representation.
 * Base on idea from
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/.
 */

#include <2lgc/math/compare_decimal.h>
#include <cstdint>
#include <cstdlib>

namespace llgc::math
{
/**
 * @brief Internal union to convert float to number (4 bytes).
 *
 * Help to count diff ULP between two float.
 */
union Float_t
{
  /**
   * @brief Constructor to initialize the default float.
   *
   * @param num The number. By default 0.0f.
   */
  explicit Float_t(float num) : f(num) {}

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
} __attribute__((packed));

/**
 * @brief Internal union to convert double to number (8 bytes).
 *
 * Help to count diff ULP between two double.
 */
union Double_t
{
  /**
   * @brief Constructor to initialize the default double.
   *
   * @param num The number. By default 0.0.
   */
  explicit Double_t(double num) : d(num) {}

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
} __attribute__((packed));
}  // namespace llgc::math

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
static bool AlmostEqualUlpsAndAbsInternal(T A, T B, T maxDiff, int maxUlpsDiff)
{
  // Check if the numbers are really close -- needed
  // when comparing numbers near zero.
  T absDiff = std::abs(A - B);
  if (absDiff <= maxDiff)
  {
    return true;
  }

  U uA(A);
  U uB(B);

  // Different signs means they do not match.
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
  if (uA.Negative() != uB.Negative())
  {
    return false;
  }

  // Find the difference in ULPs.
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
  V ulpsDiff = std::abs(uA.i - uB.i);
  return ulpsDiff <= maxUlpsDiff;
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
static bool AlmostEqualRelativeAndAbsInternal(T A, T B, T maxDiff, T maxRelDiff)
{
  // Check if the numbers are really close -- needed
  // when comparing numbers near zero.
  T diff = std::abs(A - B);
  if (diff <= maxDiff)
  {
    return true;
  }

  A = std::abs(A);
  B = std::abs(B);
  T largest = (B > A) ? B : A;

  return diff <= largest * maxRelDiff;
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
bool llgc::math::Compare::AlmostEqualUlpsAndAbsF(float A, float B,
                                                 float maxDiff, int maxUlpsDiff)
{
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
bool llgc::math::Compare::AlmostEqualRelativeAndAbsF(float A, float B,
                                                     float maxDiff,
                                                     float maxRelDiff)
{
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
bool llgc::math::Compare::AlmostEqualUlpsAndAbsD(double A, double B,
                                                 double maxDiff,
                                                 int maxUlpsDiff)
{
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
bool llgc::math::Compare::AlmostEqualRelativeAndAbsD(double A, double B,
                                                     double maxDiff,
                                                     double maxRelDiff)
{
  return AlmostEqualRelativeAndAbsInternal<double>(A, B, maxDiff, maxRelDiff);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
