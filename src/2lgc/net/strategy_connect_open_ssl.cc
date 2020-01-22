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

#ifndef NET_STRATEGY_CONNECT_OPEN_SSL_CC_
#define NET_STRATEGY_CONNECT_OPEN_SSL_CC_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/net/openssl.h>
// IWYU wants to remove it. But without you can't define method.
#include <2lgc/net/strategy_connect_open_ssl.h>  // IWYU pragma: keep
#include <2lgc/pattern/strategy.h>
#include <openssl/err.h>
#include <openssl/ossl_typ.h>
#include <openssl/ssl.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorPublisherTcp;
}

/** \class llgc::net::StrategyConnectOpenSsl
 * \brief Strategy for server to receive data from TCP with OpenSSL encryption.
 * \tparam T Message from protobuf.
 */

/** \brief Constructor for Strategy server TCP with OpenSSL encryption.
 * \param[in] server The TCP server.
 * \param[in] client_sock Socket client to communicate by TCP with OpenSSL
 *            encryption.
 * \param[in] presentation The encryption method.
 * \param[in] ctx The context of SSL.
 * \param[in] ssl The SSL connexion state.
 */
template <typename T>
INLINE_TEMPLATE llgc::net::StrategyConnectOpenSsl<T>::StrategyConnectOpenSsl(
    llgc::pattern::publisher::ConnectorPublisherTcp<T> *server,
    int *client_sock, llgc::net::OpenSsl::Presentation presentation,
    std::weak_ptr<SSL_CTX> ctx, std::weak_ptr<SSL> ssl)
    : llgc::pattern::Strategy<
          llgc::pattern::publisher::ConnectorPublisherTcp<T>>(server),
      client_sock_(client_sock),
      presentation_(presentation),
      ctx_(std::move(ctx)),
      ssl_(std::move(ssl))
{
}

/** \fn llgc::net::StrategyConnectOpenSsl::StrategyConnectOpenSsl(StrategyConnectOpenSsl&& other)
 * \brief Delete move constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn llgc::net::StrategyConnectOpenSsl::StrategyConnectOpenSsl(StrategyConnectOpenSsl const& other)
 * \brief Delete copy constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn StrategyConnectOpenSsl& llgc::net::StrategyConnectOpenSsl::operator=(StrategyConnectOpenSsl&& other)
 * \brief Delete move operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn StrategyConnectOpenSsl& llgc::net::StrategyConnectOpenSsl::operator=(StrategyConnectOpenSsl const& other)
 * \brief Delete copy operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn llgc::net::StrategyConnectOpenSsl::~StrategyConnectOpenSsl()
 * \brief Default destructor.
 */

/** \brief Function that will be executed by the thread that wait instruction
 *         from a client.
 * \return true if no problem.
 */
template <typename T>
INLINE_TEMPLATE bool llgc::net::StrategyConnectOpenSsl<T>::Do()
{
  llgc::net::Linux::AutoCloseSocket auto_close_socket(client_sock_);

  if (auto ssl = ssl_.lock())
  {
    do
    {
      char client_message[1500];

      std::cout << "Connected with " << SSL_get_cipher(ssl.get()) << " encryption" << std::endl;
      std::cout << "Client wait." << std::endl;

      int read_size =
          SSL_read(ssl.get(), client_message, sizeof(client_message));

      // Empty message.
      if (read_size == 0)
      {
        std::cout << "Client empty." << std::endl;
        continue;
      }

      std::cout << "Client received." << std::endl;

      BUGLIB(
          std::cout, read_size > 0,
          (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
          "OpenSSL");

      T messages;
      std::string client_string(client_message, static_cast<size_t>(read_size));
      BUGLIB(std::cout, messages.ParseFromString(client_string), false,
             "protobuf");

      BUGCONT(std::cout, this->instance_->GetSubscriber()->Listen(messages),
              false);
    } while (!this->instance_->GetDisposing());
  }
  else
  {
    BUGCRIT(std::cout, false, false, "Failed to lock ssl.");
  }
  return true;
}

/** \var llgc::net::StrategyConnectOpenSsl::client_sock_
 * \brief Value of the socket. Turn back to -1 when Do() finishes.
 *
 *
 * \var llgc::net::StrategyConnectOpenSsl::presentation_
 * \brief Type of encryption.
 *
 *
 * \var llgc::net::StrategyConnectOpenSsl::ctx_
 * \brief The context for SSL.
 *
 *
 * \var llgc::net::StrategyConnectOpenSsl::ssl_
 * \brief SSL connection state.
 */

#endif  // NET_STRATEGY_CONNECT_OPEN_SSL_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
