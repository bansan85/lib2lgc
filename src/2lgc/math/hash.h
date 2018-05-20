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
