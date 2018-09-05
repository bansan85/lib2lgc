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

/** \file compare_decimal.cc
 * \brief Compare diff of two decimal number base on their int representation.
 * Base on idea from
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/.
 */

#include <2lgc/math/compare_decimal.h>
// No for GCC-8. GCC-6 wants. for abs(float/double)
#include <cmath>  // IWYU pragma: keep
#include <cstdint>
#include <cstdlib>  // for abs(int/long)

/** \class llgc::math::Compare
 * \brief Class that extends math functionnality.
 */

namespace llgc::math
{
namespace
{
/** \brief Internal union to convert float to number (4 bytes).
 * Help to count diff ULP between two float.
 */
union Float_t
{
  static_assert(sizeof(float) == 4, "Size of float must be 4.");
  /** \brief Constructor to initialize the default float.
   * \param[in] num The number. By default 0.0f.
   */
  explicit Float_t(float num) : f(num) {}

  /** \brief Is the float number negative ?
   * \return true if float is negative.
   */
  bool Negative() const { return i < 0; }

  /// \brief The float number.
  float f;

  /// \brief The int representation of the float number.
  int32_t i;
} __attribute__((packed));

/** \brief Internal union to convert double to number (8 bytes).
 * Help to count diff ULP between two double.
 */
union Double_t
{
  static_assert(sizeof(double) == 8, "Size of double must be 8.");
  /** \brief Constructor to initialize the default double.
   * \param[in] num The number. By default 0.0.
   */
  explicit Double_t(double num) : d(num) {}

  /** \brief Is the double number negative ?
   * \return true if double is negative.
   */
  bool Negative() const { return i < 0; }

  /// \brief The double number.
  double d;

  /// \brief The int representation of the double number.
  int64_t i;
} __attribute__((packed));

/** \brief Function that compare equality of two decimal number that are not
 *         close to zero. Internal function. Use AlmostEqualUlpsAndAbsF or
 *         AlmostEqualUlpsAndAbsD instead.
 * \tparam T float or double.
 * \tparam U Float_t or Double_t.
 * \tparam V int32_t or int64_t.
 * \param[in] a Number to compare from.
 * \param[in] b Number to compare with.
 * \param[in] maxDiff If diff between a and b is less than maxDiff, both numbers
 *       are equals.
 * \param[in] maxUlpsDiff If diff between int representation of a and b is less
 *       than maxUlpsDiff, both number are equals.
 * \return true is both numbers are equals.
 */
template <typename T, typename U, typename V>
bool AlmostEqualUlpsAndAbsInternal(T a, T b, T maxDiff, int maxUlpsDiff)
{
  // Check if the numbers are really close -- needed
  // when comparing numbers near zero.
  T absDiff = std::abs(a - b);
  if (absDiff <= maxDiff)
  {
    return true;
  }

  U uA(a);
  U uB(b);

  // Different signs means they can't match.
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

/** \brief Function that compare equality of two decimal number that are close
 *         to zero.
 * \tparam T float or double.
 * \param[in] a Number to compare from.
 * \param[in] b Number to compare with.
 * \param[in] maxDiff If diff between a and b is less than maxDiff, both numbers
 *       are equals.
 * \param[in] maxRelDiff If diff between the two numbers is smaller than
 *       maxRelDiff the max of the two numbers, both number are equals.
 * \return true is both numbers are equals.
 */
template <typename T>
bool AlmostEqualRelativeAndAbsInternal(T a, T b, T maxDiff, T maxRelDiff)
{
  // Check if the numbers are really close. It's needed when comparing numbers
  // near zero.
  T diff = std::abs(a - b);
  if (diff <= maxDiff)
  {
    return true;
  }

  a = std::abs(a);
  b = std::abs(b);
  T largest = (b > a) ? b : a;

  return diff <= largest * maxRelDiff;
}

}  // namespace
}  // namespace llgc::math

/// \file compare_decimal.h

/** \brief Compare two float numbers that are not close to zero.
 * \param[in] a Number to compare from.
 * \param[in] b Number to compare with.
 * \param[in] maxDiff If diff between a and b is less than maxDiff, both numbers
 *       are equals.
 * \param[in] maxUlpsDiff If diff between int representation of a and b is less
 *       than maxUlpsDiff, both number are equals.
 * \return true is both numbers are equals.
 */
bool llgc::math::Compare::AlmostEqualUlpsAndAbsF(float a, float b,
                                                 float maxDiff, int maxUlpsDiff)
{
  return AlmostEqualUlpsAndAbsInternal<float, Float_t, int32_t>(a, b, maxDiff,
                                                                maxUlpsDiff);
}

/** \brief Compare two float numbers that are close to zero.
 * \param[in] a Number to compare from.
 * \param[in] b Number to compare with.
 * \param[in] maxDiff If diff between a and b is less than maxDiff, both numbers
 *       are equals.
 * \param[in] maxRelDiff If diff between the two numbers is smaller than
 *       maxRelDiff the max of the two numbers, both number are equals.
 * \return true is both numbers are equals.
 */
bool llgc::math::Compare::AlmostEqualRelativeAndAbsF(float a, float b,
                                                     float maxDiff,
                                                     float maxRelDiff)
{
  return AlmostEqualRelativeAndAbsInternal<float>(a, b, maxDiff, maxRelDiff);
}

/** \brief Compare two double numbers that are not close to zero.
 * \param[in] a Number to compare from.
 * \param[in] b Number to compare with.
 * \param[in] maxDiff If diff between a and b is less than maxDiff, both numbers
 *       are equals.
 * \param[in] maxUlpsDiff If diff between int representation of a and b is less
 *       than maxUlpsDiff, both number are equals.
 * \return true is both numbers are equals.
 */
bool llgc::math::Compare::AlmostEqualUlpsAndAbsD(double a, double b,
                                                 double maxDiff,
                                                 int maxUlpsDiff)
{
  return AlmostEqualUlpsAndAbsInternal<double, Double_t, int64_t>(a, b, maxDiff,
                                                                  maxUlpsDiff);
}

/** \brief Compare two double numbers that are close to zero.
 * \param[in] a Number to compare from.
 * \param[in] b Number to compare with.
 * \param[in] maxDiff If diff between `a` and `b` is less than `maxDiff`, both
 *       numbers are equals.
 * \param[in] maxRelDiff If diff between the two numbers is smaller than
 *       `maxRelDiff` the max of the two numbers, both number are equals.
 * \return true is both numbers are equals.
 */
bool llgc::math::Compare::AlmostEqualRelativeAndAbsD(double a, double b,
                                                     double maxDiff,
                                                     double maxRelDiff)
{
  return AlmostEqualRelativeAndAbsInternal<double>(a, b, maxDiff, maxRelDiff);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
