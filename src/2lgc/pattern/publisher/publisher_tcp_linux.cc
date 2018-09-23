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

#ifndef PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_CC_
#define PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_CC_

#include <2lgc/compat.h>
#include <2lgc/config.h>
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/net/openssl.h>
// std::make_unique needs it.
#include <2lgc/net/strategy_listen_open_ssl.h>  // IWYU pragma: keep
// std::make_unique needs it.
#include <2lgc/net/strategy_listen_tcp_linux.h>  // IWYU pragma: keep
#include <2lgc/pattern/publisher/publisher_tcp.h>
// IWYU wants to remove it. But without you can't define method.
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>  // IWYU pragma: keep
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <cerrno>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

namespace llgc::pattern::publisher
{
template <typename T>
class ConnectorSubscriberTcp;

template <typename T>
class SubscriberServerTcp;
}  // namespace llgc::pattern::publisher

namespace llgc::pattern
{
template <class T>
class Strategy;
}  // namespace llgc::pattern

/** \enum llgc::net::OpenSsl::Presentation
 * \brief Type of encryption. NONE do not use OpenSSL.
 *
 *
 * \class llgc::pattern::publisher::PublisherTcpLinux
 * \brief Interface to create a TCP server.
 *
 *
 * \class llgc::net::OpenSsl::Presentation
 * \brief List of encryption method supported.
 */

/** \brief Constructor with port for the TCP server.
 * \param[in] port The port to listen from.
 */
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::PublisherTcpLinux<T>::PublisherTcpLinux(uint16_t port)
    : PublisherTcp<T>(port)
{
}

template <typename T>
INLINE_TEMPLATE bool llgc::pattern::publisher::PublisherTcpLinux<T>::Wait()
{
  if (sockfd_ == -1)
  {
    return false;
  }

  std::thread t([this]() {
    int iResult;
    int client_sock = 0;
    std::unique_ptr<llgc::pattern::Strategy<PublisherTcpLinux<T>>> receiver_;
    fd_set rfds;

    do
    {
      FD_ZERO(&rfds);  // NOLINT(hicpp-no-assembler)
      // NOLINTNEXTLINE(hicpp-signed-bitwise, cppcoreguidelines-pro-bounds-constant-array-index, cppcoreguidelines-pro-type-member-init)
      FD_SET(sockfd_, &rfds);
      // NOLINTNEXTLINE(hicpp-member-init, cppcoreguidelines-pro-type-member-init)
      struct timeval tv;
      tv.tv_sec = 0L;
      tv.tv_usec = 50000L;

      iResult = llgc::net::Linux::RepeteOnEintr([this, &rfds, &tv] {
        return select(sockfd_ + 1, &rfds, nullptr, nullptr, &tv);
      });
      if (iResult > 0)
      {
        client_sock = llgc::net::Linux::RepeteOnEintr(
            [=] { return accept4(sockfd_, nullptr, nullptr, SOCK_CLOEXEC); });
        if (client_sock > 0)
        {
#ifdef OPENSSL_FOUND
          if (presentation_ != llgc::net::OpenSsl::Presentation::NONE)
          {
            BUGCONT(std::cout,
                    llgc::net::OpenSsl::InitCtxSslServer(presentation_, &ctx_,
                                                         &ssl_), );
            receiver_ = std::make_unique<llgc::net::StrategyListenOpenSsl<T>>(
                this, &client_sock, presentation_, cert_, key_, ctx_, ssl_);
          }
          else
#endif  // OPENSSL_FOUND
          {
            receiver_ = std::make_unique<llgc::net::StrategyListenTcpLinux<
                T, llgc::pattern::publisher::PublisherTcpLinux<T>>>(
                this, &client_sock,
                [this, client_sock](const T &messages) -> bool {
                  return this->PreForward(messages, client_sock);
                });
          }
          std::thread t2([&receiver_] { return receiver_->Do(); });
          this->thread_sockets_.insert(
              std::pair<int, std::thread>(client_sock, std::move(t2)));
        }
      }
    } while (iResult >= 0 && client_sock >= 0 && !this->disposing_);
    BUGCRIT(std::cout, iResult != -1, ,
            "Errno " + std::to_string(errno) + "\n");
  });

  this->thread_wait_ = std::move(t);

  return true;
}

/** @brief Add / remove subscriber in server before sending message to
 *         subscriber.
 * @param[in] messages List of messages to forward to subscriber.
 * @param[in] socket Socket to communicate with the subscriber.
 * @return true if no problem.
 */
template <typename T>
INLINE_TEMPLATE bool llgc::pattern::publisher::PublisherTcpLinux<T>::PreForward(
    const T &messages, int socket)
{
  for (int i = 0; i < messages.msg_size(); i++)
  {
    const typename T::Msg &message = messages.msg(i);

    typename T::Msg::DataCase enumeration = message.data_case();

    if (enumeration == T::Msg::kAddSubscriber)
    {
      AddSubscriberLocal(socket, message);
    }
    else if (enumeration == T::Msg::kRemoveSubscriber)
    {
      // We need to create a temporary connector with the socket
      // because the Remover only works with connector.
      std::shared_ptr<llgc::pattern::publisher::SubscriberServerTcp<T>>
          subscriber = std::make_shared<
              llgc::pattern::publisher::SubscriberServerTcp<T>>(socket);
      std::shared_ptr<llgc::pattern::publisher::ConnectorSubscriberTcp<T>>
          connector = std::make_shared<
              llgc::pattern::publisher::ConnectorSubscriberTcp<T>>(subscriber,
                                                                   socket);
      BUGCONT(std::cout,
              this->RemoveSubscriber(message.remove_subscriber().id_message(),
                                     connector),
              false);
    }
  }

  BUGCONT(std::cout, this->Forward(messages), false);

  return true;
}

#ifdef OPENSSL_FOUND
/** \brief Set parameter for OpenSSL server.
 * \param[in] presentation Choose the encryption method.
 * \param[in] cert Set the certificate file.
 * \param[in] key Set the key file.
 */
template <typename T>
INLINE_TEMPLATE void
llgc::pattern::publisher::PublisherTcpLinux<T>::SetEncryption(
    llgc::net::OpenSsl::Presentation presentation, const std::string &cert,
    const std::string &key)
{
  presentation_ = presentation;
  cert_ = cert;
  key_ = key;
}
#endif

/** \var llgc::pattern::publisher::PublisherTcpLinux::sockfd_
 * \brief Socket file descriptor.
 */

template <typename T>
INLINE_TEMPLATE void
llgc::pattern::publisher::PublisherTcpLinux<T>::AddSubscriberLocal(
    int socket, const typename T::Msg &message)
{
  BUGCRIT(std::cout, message.has_add_subscriber(), ,
          "Failed to add a subscriber.");
  std::shared_ptr<llgc::pattern::publisher::SubscriberServerTcp<T>> subscriber =
      std::make_shared<llgc::pattern::publisher::SubscriberServerTcp<T>>(
          socket);
  std::shared_ptr<llgc::pattern::publisher::ConnectorSubscriberTcp<T>>
      connector =
          std::make_shared<llgc::pattern::publisher::ConnectorSubscriberTcp<T>>(
              subscriber, socket);
  BUGCONT(
      std::cout,
      this->AddSubscriber(message.add_subscriber().id_message(), connector), );
}

/** \var llgc::pattern::publisher::PublisherTcpLinux::presentation_
 * \brief Tell if encryption is used.
 *
 *
 * \var llgc::pattern::publisher::PublisherTcpLinux::cert_
 * \brief The certification file for encryption.
 *
 *
 * \var llgc::pattern::publisher::PublisherTcpLinux::key_
 * \brief The key file for encryption.
 */

#endif  // PATTERN_PUBLISHER_PUBLISHER_TCP_LINUX_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
