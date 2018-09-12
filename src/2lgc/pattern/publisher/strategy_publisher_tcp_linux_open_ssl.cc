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

#ifndef PATTERN_PUBLISHER_STRATEGY_PUBLISHER_TCP_LINUX_OPEN_SSL_CC_
#define PATTERN_PUBLISHER_STRATEGY_PUBLISHER_TCP_LINUX_OPEN_SSL_CC_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/net/openssl.h>
#include <2lgc/pattern/publisher/strategy_publisher_tcp_linux_open_ssl.h>
#include <2lgc/pattern/strategy.h>
#include <openssl/ossl_typ.h>
#include <openssl/ssl.h>
#include <stddef.h>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

/** \class llgc::pattern::publisher::StrategyPublisherTcpLinuxOpenSsl
 * \brief Strategy for server to receive data from TCP with OpenSSL encryption.
 * \tparam T Message from protobuf.
 */

/** \brief Constructor for Strategy server TCP with OpenSSL encryption.
 * \param[in] server The TCP server.
 * \param[in] client_sock Socket client to communicate by TCP with OpenSSL
 *            encryption.
 * \param[in] presentation The encryption method.
 * \param[in] cert The certificate file.
 * \param[in] key The key file.
 */
template <typename T>
INLINE_TEMPLATE llgc::pattern::publisher::StrategyPublisherTcpLinuxOpenSsl<T>::
    StrategyPublisherTcpLinuxOpenSsl(
        llgc::pattern::publisher::PublisherTcpLinux<T> *server,
        int &client_sock, llgc::net::OpenSsl::Presentation presentation,
        const std::string &cert, const std::string &key)
    : llgc::pattern::Strategy<llgc::pattern::publisher::PublisherTcpLinux<T>>(
          server),
      client_sock_(client_sock),
      presentation_(presentation),
      cert_(cert),
      key_(key)
{
}

/** \fn llgc::pattern::publisher::StrategyPublisherTcpLinuxOpenSsl::~StrategyPublisherTcpLinuxOpenSsl()
 * \brief Default destructor.
 */

/** \brief Function that will be executed by the thread that wait instruction
 *         from a client.
 * \return true if no problem.
 */
template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::StrategyPublisherTcpLinuxOpenSsl<T>::Do()
{
  llgc::net::Linux::AutoCloseSocket auto_close_socket(&client_sock_);

  const SSL_METHOD *method;
  switch (presentation_)
  {
    case llgc::net::OpenSsl::Presentation::TSL1_2:
    {
      method = TLSv1_2_server_method();
      break;
    }
    case llgc::net::OpenSsl::Presentation::NONE:
    default:
    {
      return false;
    }
  }

  std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX *)>> ctx(
      SSL_CTX_new(method), [](SSL_CTX *ptr) { SSL_CTX_free(ptr); });

  BUGLIB(std::cout, ctx != nullptr, false, "OpenSSL");

  BUGLIB(std::cout,
         SSL_CTX_use_certificate_file(ctx.get(), cert_.c_str(),
                                      SSL_FILETYPE_PEM) == 1,
         false, "OpenSSL");
  BUGLIB(std::cout,
         SSL_CTX_use_PrivateKey_file(ctx.get(), key_.c_str(),
                                     SSL_FILETYPE_PEM) == 1,
         false, "OpenSSL");
  BUGUSER(std::cout, SSL_CTX_check_private_key(ctx.get()), false,
          "Private key does not match the public certificate\n");

  std::unique_ptr<SSL, std::function<void(SSL *)>> ssl(
      SSL_new(ctx.get()), [](SSL *ptr) { SSL_free(ptr); });

  SSL_set_fd(ssl.get(), client_sock_);
  BUGCRIT(std::cout, SSL_accept(ssl.get()) != 1, false,
          "Failed to initialize handshake.\n");

  do
  {
    char client_message[1500];

    int read_size = SSL_read(ssl.get(), client_message, sizeof(client_message));

    // Empty message.
    if (read_size == 0)
    {
      continue;
    }

    BUGCRIT(std::cout, read_size > 0, false,
            "Server OpenSSL client " + std::to_string(client_sock_) +
                " recv failed\n.");

    T messages;
    std::string client_string(client_message, static_cast<size_t>(read_size));
    BUGLIB(std::cout, messages.ParseFromString(client_string), false,
           "protobuf");

    BUGCONT(std::cout, this->instance_->PreForward(messages, client_sock_),
            false);
  } while (!this->instance_->GetDisposing());
  return true;
}

/** \var llgc::pattern::publisher::StrategyPublisherTcpLinuxOpenSsl::client_sock_
 * \brief Value of the socket. Turn back to -1 when Do() finishes.
 *
 *
 * \var llgc::pattern::publisher::StrategyPublisherTcpLinuxOpenSsl::presentation_
 * \brief Type of encryption.
 *
 *
 * \var llgc::pattern::publisher::StrategyPublisherTcpLinuxOpenSsl::cert_
 * \brief Certification file if OpenSSL is used.
 *
 *
 * \var llgc::pattern::publisher::StrategyPublisherTcpLinuxOpenSsl::key_
 * \brief Key file if OpenSSL is used.
 */

#endif  // PATTERN_PUBLISHER_STRATEGY_PUBLISHER_TCP_LINUX_OPEN_SSL_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
