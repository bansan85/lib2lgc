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

/** \file hash.h
 * \brief Use function hash based on OpenSSL presence.
 */

#include <2lgc/config.h>
#include <2lgc/error/show.h>
#include <2lgc/math/hash.h>
#include <iostream>

#ifdef OPENSSL_FOUND
#include <openssl/sha.h>
#endif

/** \class llgc::math::Hash
 * \brief Class that automatically choose the hash function.
 */

/** \brief Compute hash.
 * \param[in] in The string to compute.
 * \return The hash in binary format.
 */
std::vector<uint8_t> llgc::math::Hash::Calc(const std::string &in)
{
#ifdef OPENSSL_FOUND
  return Sha512(in);
#else
  return Jdb2(in);
#endif
}

/** \brief Compute Jdb2 hash.
 * \param[in] in The string to compute.
 * \return The hash in binary format.
 */
std::vector<uint8_t> llgc::math::Hash::Jdb2(const std::string &in)
{
  std::vector<uint8_t> retval(8);
  uint64_t hash = 5381;

  for (int8_t c : in)
  {
    hash = ((hash << 5U) + hash) + static_cast<uint8_t>(c);
  }

  for (uint8_t i = 0; i < 8; i++)
  {
    retval[i] = (hash >> (i * 8U)) & 0xFFU;
  }

  return retval;
}

/** \brief Compute Sha512 hash.
 * \param[in] in The string to compute.
 * \return The hash in binary format.
 */
#ifdef OPENSSL_FOUND
std::vector<uint8_t> llgc::math::Hash::Sha512(const std::string &in)
{
  uint8_t hash[SHA512_DIGEST_LENGTH];

  // Don't use SHA512 that need an reinterpret_cast because c_str() is
  // const char* and SHA512 in const unsigned char*.
  SHA512_CTX ctx;
  BUGLIB(std::cout, SHA512_Init(&ctx) == 1, std::vector<uint8_t>(), "openssl");
  BUGLIB(std::cout, SHA512_Update(&ctx, in.c_str(), in.length()) == 1,
         std::vector<uint8_t>(), "openssl");
  BUGLIB(std::cout, SHA512_Final(hash, &ctx) == 1, std::vector<uint8_t>(),
         "openssl");
  return std::vector<uint8_t>(hash, hash + SHA512_DIGEST_LENGTH);
}
#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
