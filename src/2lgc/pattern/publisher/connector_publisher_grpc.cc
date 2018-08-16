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

#include <2lgc/error/show.h>
#include <2lgc/net/linux.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_publisher_grpc.h>
#include <2lgc/poco/pattern_publisher.pb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/create_channel.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <cerrno>
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>

template <typename T, typename S>
llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::ConnectorPublisherGrpc(
    std::shared_ptr<SubscriberInterface<T>> subscriber, std::string ip,
    uint16_t port)
    : ConnectorInterface<T>(subscriber),
      ip_(std::move(ip)),
      port_(port),
      stub_(nullptr),
      disposing_(false)
{
}

template <typename T, typename S>
llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::~ConnectorPublisherGrpc()
{
  // Properly stop recv and close file descriptor.
  disposing_ = true;
  if (receiver_.joinable())
  {
    receiver_.join();
  }
}

template <typename T, typename S>
bool llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Equals(
    const ConnectorInterface<T> &connector) const
{
  const auto *connector_direct =
      dynamic_cast<const ConnectorPublisherGrpc<T, S> *>(&connector);

  if (connector_direct == nullptr)
  {
    return false;
  }

  return this->subscriber_->Equals(*connector.GetSubscriber());
}

template <typename T, typename S>
bool llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::AddSubscriber(
    uint32_t id_message)
{
  BUGCONT(std::cout, Connect(), false);

  T messages;
  auto message = messages.add_msg();
  auto add =
      std::make_unique<llgc::protobuf::pattern::publisher::AddSubscriber>();
  add->set_id_message(id_message);
  message->set_allocated_add_subscriber(add.release());
  std::string messages_in_string;
  BUGLIB(std::cout, messages.SerializeToString(&messages_in_string), false,
         "protobuf");
  BUGCONT(std::cout, Send(messages_in_string), false);

  return true;
}

template <typename T, typename S>
bool llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Send(
    const std::string &message)
{
  std::cout << "ConnectorPublisherGrpc<T, S>::Send" << std::endl;
  std::cout << "Client  -> serveur " << message.length() << std::endl;
//  BUGCONT(std::cout, llgc::net::Linux::Send(socket_, message), false);
  return true;
}

template <typename T, typename S>
bool llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::RemoveSubscriber(
    uint32_t id_message)
{
  BUGCONT(std::cout, Connect(), false);

  T messages;
  auto message = messages.add_msg();
  auto message_add =
      std::make_unique<llgc::protobuf::pattern::publisher::RemoveSubscriber>();
  message_add->set_id_message(id_message);
  message->set_allocated_remove_subscriber(message_add.release());
  std::string messages_in_string;
  BUGLIB(std::cout, messages.SerializeToString(&messages_in_string), false,
         "protobuf");
  BUGCONT(std::cout, Send(messages_in_string), false);

  return true;
}

template <typename T, typename S>
void llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Receiver()
{
  std::cout << "Client grpc Start" << std::endl;

  T message;
  while (!disposing_ && stream_->Read(&message))
  {
    BUGCONT(std::cout, this->subscriber_->Listen(message), );
  }

  std::cout << "Client grpc : end" << std::endl;

  this->stub_.reset();
}

template <typename T, typename S>
bool llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Connect()
{
  if (stub_ != nullptr)
  {
    return true;
  }

  std::stringstream ss;

  ss << ip_ << ":" << port_;
  stub_ = S::NewStub(grpc::CreateChannel(ss.str(), grpc::InsecureChannelCredentials()));
  stream_ = stub_->Talk(&context);


  std::thread t(&ConnectorPublisherGrpc<T, S>::Receiver, this);
  this->receiver_ = std::move(t);

  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
