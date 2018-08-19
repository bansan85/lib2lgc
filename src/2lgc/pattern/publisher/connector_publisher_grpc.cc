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
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_publisher_grpc.h>
#include <2lgc/poco/pattern_publisher.pb.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

template <typename T, typename S>
llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::ConnectorPublisherGrpc(
    std::shared_ptr<SubscriberInterface<T>> subscriber, std::string ip,
    uint16_t port)
    : ConnectorInterface<T>(subscriber),
      ip_(std::move(ip)),
      port_(port),
      stub_(nullptr)
{
}

template <typename T, typename S>
llgc::pattern::publisher::ConnectorPublisherGrpc<T,
                                                 S>::~ConnectorPublisherGrpc()
{
  // Properly stop recv and close file descriptor.
  context_.TryCancel();
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
  BUGCONT(std::cout, Send(messages), false);

  return true;
}

template <typename T, typename S>
bool llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Send(
    const T &message)
{
  BUGCONT(std::cout, Connect(), false);

  BUGLIB(std::cout, stream_->Write(message), false, "grpc");

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
  BUGCONT(std::cout, Send(messages), false);

  return true;
}

template <typename T, typename S>
void llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Receiver()
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

template <typename T, typename S>
bool llgc::pattern::publisher::ConnectorPublisherGrpc<T, S>::Connect()
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

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
