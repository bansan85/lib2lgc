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
 * @file hash.h
 * @brief Use function hash based on OpenSSL presence.
 */

#ifndef MATH_HASH_H_
#define MATH_HASH_H_

#include <cstdint>
#include <string>
#include <vector>

namespace llgc::math
{
/**
 * @brief Class that automatically choose the hash function.
 */
class Hash
{
 public:
  /**
   * @brief Compute hash.
   *
   * @param[in] in The string to compute.
   *
   * @return The hash in binary format.
   */
  static std::vector<uint8_t> Calc(const std::string& in);

 private:
  /**
   * @brief Compute Jdb2 hash.
   *
   * @param[in] in The string to compute.
   *
   * @return The hash in binary format.
   */
  static std::vector<uint8_t> Jdb2(const std::string& in);

  /**
   * @brief Compute Sha512 hash.
   *
   * @param[in] in The string to compute.
   *
   * @return The hash in binary format.
   */
  static std::vector<uint8_t> Sha512(const std::string& in);
};

}  // namespace llgc::math

#endif  // MATH_HASH_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
