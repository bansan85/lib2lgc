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

#ifndef NET_OPENSSL_H_
#define NET_OPENSSL_H_

#include <2lgc/config.h>

#ifdef OPENSSL_FOUND

#include <openssl/ssl.h>

namespace llgc::net
{
class OpenSsl
{
 public:
  static bool Init() CHK;

 private:
  static std::mutex mutex_;
  static bool init_done;
};

}  // namespace llgc::net

#endif  // OPENSSL_FOUND

#endif  // NET_LINUX_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
