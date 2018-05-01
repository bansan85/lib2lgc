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
 * @file hash.cc
 * @brief Use function hash based on OpenSSL presence.
 */

#include <2lgc/config.h>
#include <2lgc/math/hash.h>

#ifdef OPENSSL_FOUND
#include <openssl/sha.h>
#endif

std::vector<uint8_t> llgc::math::Hash::Calc(const std::string& in)
{
#ifdef OPENSSL_FOUND
  return Sha512(in);
#else
  return Jdb2(in);
#endif
}

std::vector<uint8_t> llgc::math::Hash::Jdb2(const std::string& in)
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

std::vector<uint8_t> llgc::math::Hash::Sha512(const std::string& in)
{
#ifdef OPENSSL_FOUND
  uint8_t hash[SHA512_DIGEST_LENGTH];

  // Don't use SHA512 that need an reinterpret_cast because c_str() is
  // const char* and SHA512 in const unsigned char*.
  SHA512_CTX ctx;
  SHA512_Init(&ctx);
  SHA512_Update(&ctx, in.c_str(), in.length());
  SHA512_Final(hash, &ctx);

  return std::vector<uint8_t>(hash, hash + SHA512_DIGEST_LENGTH);
#else
  return std::vector<uint8_t>();
#endif
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
