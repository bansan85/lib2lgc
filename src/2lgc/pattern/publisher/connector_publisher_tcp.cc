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

#ifndef PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_CC_
#define PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_CC_

#include <2lgc/compat.h>
#include <2lgc/config.h>
#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/net/openssl.h>
#include <2lgc/pattern/publisher/connector_interface.h>
// IWYU wants to remove it. But without you can't define method.
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>  // IWYU pragma: keep
#include <2lgc/poco/pattern_publisher.pb.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;
}

/** \class llgc::pattern::publisher::ConnectorPublisherTcp
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 */

/** \brief Default constructor.
 * \param[in] subscriber The subscriber.
 * \param[in] ip The IP of the server.
 * \param[in] port The port of the server.
 */
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::ConnectorPublisherTcp<T>::ConnectorPublisherTcp(
    std::shared_ptr<SubscriberInterface<T>> subscriber, std::string ip,
    uint16_t port)
    : ConnectorInterface<T>(subscriber),
      socket_(-1),
      ip_(std::move(ip)),
      port_(port),
      disposing_(false)
{
}

/** \fn llgc::pattern::publisher::ConnectorPublisherTcp::ConnectorPublisherTcp(ConnectorPublisherTcp &&other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorPublisherTcp::ConnectorPublisherTcp(ConnectorPublisherTcp const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn ConnectorPublisherTcp & llgc::pattern::publisher::ConnectorPublisherTcp::operator=(ConnectorPublisherTcp &&other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn ConnectorPublisherTcp & llgc::pattern::publisher::ConnectorPublisherTcp::operator=(ConnectorPublisherTcp const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

/// \brief Default virtual destructor.
template <typename T>
INLINE_TEMPLATE
    llgc::pattern::publisher::ConnectorPublisherTcp<T>::~ConnectorPublisherTcp()
{
  // Properly stop recv and close file descriptor.
  disposing_ = true;
  if (receiver_.joinable())
  {
    receiver_.join();
  }
}

/** \brief Send message to the publisher.
 * \param[in] message The protobuf message.
 * \return true if no problem.
 * \dotfile pattern/publisher/publisher_tcp_send_message.dot
 */
template <typename T>
INLINE_TEMPLATE bool llgc::pattern::publisher::ConnectorPublisherTcp<T>::Send(
    const T &message)
{
  BUGCONT(std::cout, Connect(), false);

  std::string message_in_string;
  BUGLIB(std::cout, message.SerializeToString(&message_in_string), false,
         "protobuf");
#ifdef OPENSSL_FOUND
  if (presentation_ != llgc::net::OpenSsl::Presentation::NONE)
  {
    BUGLIB(std::cout,
           SSL_write(ssl_.get(), message_in_string.c_str(),
                     static_cast<int>(message_in_string.length())) > 0,
           (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
           "OpenSSL");
  }
  else
#endif
  {
    BUGCONT(std::cout, llgc::net::Linux::Send(socket_, message_in_string),
            false);
  }
  return true;
}

template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorPublisherTcp<T>::AddSubscriber(
    uint32_t id_message)
{
  BUGCONT(std::cout, Connect(), false);

  T messages;
  auto message = messages.add_msg();
  auto add =
      std::make_unique<llgc::protobuf::pattern::publisher::AddSubscriber>();
  add->set_id_message(id_message);
  message->set_allocated_add_subscriber(add.release());
  BUGCONT(std::cout, Send(messages), false);

  return true;
}

template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorPublisherTcp<T>::RemoveSubscriber(
    uint32_t id_message)
{
  BUGCONT(std::cout, Connect(), false);

  T messages;
  auto message = messages.add_msg();
  auto message_add =
      std::make_unique<llgc::protobuf::pattern::publisher::RemoveSubscriber>();
  message_add->set_id_message(id_message);
  message->set_allocated_remove_subscriber(message_add.release());
  BUGCONT(std::cout, Send(messages), false);

  return true;
}

/** \brief Tell if instance is stopped.
 * \return true if publisher is disposing.
 */
template <typename T>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorPublisherTcp<T>::GetDisposing() const
{
  return disposing_;
}

#ifdef OPENSSL_FOUND
/** \brief Set parameter for OpenSSL server.
 * \param[in] presentation Choose the encryption method.
 * \param[in] cert Set the certificate file.
 * \param[in] key Set the key file.
 */
template <typename T>
INLINE_TEMPLATE void
llgc::pattern::publisher::ConnectorPublisherTcp<T>::SetEncryption(
    llgc::net::OpenSsl::Presentation presentation, const std::string &cert,
    const std::string &key)
{
  presentation_ = presentation;
  cert_ = cert;
  key_ = key;
}
#endif

/** \var llgc::pattern::publisher::ConnectorPublisherTcp::receiver_
 * \brief Thread that listen the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::socket_
 * \brief Socket to the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::presentation_
 * \brief Type of encryption.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::cert_
 * \brief Certification file if OpenSSL is used.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::key_
 * \brief Key file if OpenSSL is used.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::ctx_
 * \brief The context for SSL.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::ssl_
 * \brief SSL connection state.
 *
 *
 * \fn bool llgc::pattern::publisher::ConnectorPublisherTcp::Connect()
 * \brief Start connection with server.
 * \return true if no problem.
 *
 *
 * \fn const std::string & llgc::pattern::publisher::ConnectorPublisherTcp::GetIp() const
 * \brief Get the ip of the connector.
 * \return The return value;
 *
 *
 * \fn uint16_t llgc::pattern::publisher::ConnectorPublisherTcp::GetPort() const
 * \brief Get the port of the connector.
 * \return The return value.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::ip_
 * \brief The IP of the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::port_
 * \brief The port of the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherTcp::disposing_
 * \brief If thread in trying to stop.
 */

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
