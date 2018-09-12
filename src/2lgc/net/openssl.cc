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

#include <2lgc/config.h>

#ifdef OPENSSL_FOUND

#include <2lgc/net/openssl.h>
#include <openssl/ssl.h>
#include <mutex>

/** \class llgc::net::OpenSsl
 * \brief Class to manipulate OpenSSL functions. All functions are thread-safe.
 */

/** \brief Run init global function.
 * \return `false` if something wrong.
 */
bool llgc::net::OpenSsl::Init()
{
  bool retval = true;
  mutex_.lock();
  retval = retval && SSL_library_init();
  OpenSSL_add_all_algorithms();
  init_done = true;
  mutex_.unlock();
  return retval;
}

/// \brief Every function is thread-safe so we need a lock.
std::mutex llgc::net::OpenSsl::mutex_;

/// \brief true if ssl library is init.
bool llgc::net::OpenSsl::init_done = false;

#endif  // OPENSSL_FOUND

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
