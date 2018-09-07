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

#ifndef PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_GRPC_CC_
#define PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_GRPC_CC_

#include <2lgc/compat.h>
#include <2lgc/error/show.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_publisher_grpc.h>
#include <2lgc/poco/pattern_publisher.pb.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

/** \class llgc::pattern::publisher::ConnectorPublisherGrpc
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 * \tparam S The gRPC server.
 */

/** \brief Default constructor.
 * \param[in] subscriber The subscriber.
 * \param[in] ip The IP of the server.
 * \param[in] port The port of the server.
 */
template <typename T, typename S>
INLINE_TEMPLATE
llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::ConnectorPublisherGrpc(
    std::shared_ptr<SubscriberInterface<T>> subscriber, std::string ip,
    uint16_t port)
    : ConnectorInterface<T>(subscriber),
      ip_(std::move(ip)),
      port_(port),
      stub_(nullptr)
{
}

/// \brief Default virtual destructor.
template <typename T, typename S>
INLINE_TEMPLATE llgc::pattern::publisher::ConnectorPublisherGrpc<
    T, S>::~ConnectorPublisherGrpc()
{
  // Properly stop recv and close file descriptor.
  context_.TryCancel();
  if (receiver_.joinable())
  {
    receiver_.join();
  }
}

template <typename T, typename S>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Send(const T &message)
{
  BUGCONT(std::cout, Connect(), false);

  BUGLIB(std::cout, stream_->Write(message), false, "grpc");

  return true;
}

template <typename T, typename S>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::AddSubscriber(
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

template <typename T, typename S>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::RemoveSubscriber(
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

/** \fn llgc::pattern::publisher::ConnectorPublisherGrpc::ConnectorPublisherGrpc(ConnectorPublisherGrpc &&other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorPublisherGrpc::ConnectorPublisherGrpc(ConnectorPublisherGrpc const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn ConnectorPublisherGrpc & llgc::pattern::publisher::ConnectorPublisherGrpc::operator=(ConnectorPublisherGrpc &&other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn ConnectorPublisherGrpc & llgc::pattern::publisher::ConnectorPublisherGrpc::operator=(ConnectorPublisherGrpc const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherGrpc::ip_
 * \brief The IP of the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherGrpc::port_
 * \brief The port of the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherGrpc::stream_
 * \brief Stream to communicate with the server.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherGrpc::channel_
 * \brief Channel of the client.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherGrpc::context_
 * \brief Context of the client.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherGrpc::stub_
 * \brief Necessary variable.
 *
 *
 * \var llgc::pattern::publisher::ConnectorPublisherGrpc::receiver_
 * \brief Thread that listen the server.
 */

/** \brief Start connection with server.
 * \return true if no problem.
 */
template <typename T, typename S>
INLINE_TEMPLATE bool
llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Connect()
{
  if (stub_ != nullptr)
  {
    return true;
  }

  std::stringstream ss;

  ss << ip_ << ":" << port_;
  channel_ = grpc::CreateChannel(ss.str(), grpc::InsecureChannelCredentials());
  stub_ = S::NewStub(channel_);
  stream_ = stub_->Talk(&context_);

  std::thread t(&ConnectorPublisherGrpc<T, S>::Receiver, this);
  this->receiver_ = std::move(t);

  return true;
}

/** \brief The function used by the thread that receive message from server.
 *         Need to be public so thread can use it. Protected is not possible.
 */
template <typename T, typename S>
INLINE_TEMPLATE void
llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Receiver()
{
  T message;
  // Will be stop with context_.TryCancel() on destructor.
  while (stream_->Read(&message))
  {
    BUGCONT(std::cout, this->subscriber_->Listen(message), );
  }

  this->stream_.reset();
  this->stub_.reset();
}

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_GRPC_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
