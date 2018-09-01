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
#include <2lgc/pattern/publisher/connector_subscriber_grpc.h>
#include <2lgc/pattern/publisher/publisher_grpc.h>
#include <2lgc/pattern/publisher/publisher_ip.h>
#include <2lgc/pattern/publisher/subscriber_server_grpc.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

/** \class llgc::pattern::publisher::PublisherGrpc
 * \brief Interface to create a TCP server.
 * \tparam T Message from protobuf.
 * \dotfile pattern/publisher/publisher_tcp.dot
 */

/** \brief Constructor with port for the TCP server.
 * \param[in] port The port to listen from.
 */
template <typename T, typename S>
llgc::pattern::publisher::PublisherGrpc<T, S>::PublisherGrpc(uint16_t port)
    : llgc::pattern::publisher::PublisherIp<T>(port)
{
}

/// \brief Destructor. Make sure that thread is finished.
template <typename T, typename S>
llgc::pattern::publisher::PublisherGrpc<T, S>::~PublisherGrpc()
{
  Stop();
}

/** \brief Wait, read and forward messages from a client to subscribers.
 * \param[in] context Don't care.
 * \param[in] stream Listen from this stream.
 * \return The return value when the stream is closed by the server.
 */
template <typename T, typename S>
grpc::Status llgc::pattern::publisher::PublisherGrpc<T, S>::Talk(
    grpc::ServerContext* context, grpc::ServerReaderWriter<T, T>* stream)
{
  // One endless thread for each connexion.
  // But same 'this' for everyone.
  T messages;
  while (stream->Read(&messages))
  {
    for (int i = 0; i < messages.msg_size(); i++)
    {
      auto& message = messages.msg(i);

      auto enumeration = message.data_case();

      if (enumeration ==
          std::remove_reference<decltype(message)>::type::kAddSubscriber)
      {
        std::shared_ptr<llgc::pattern::publisher::SubscriberServerGrpc<T>>
            subscriber = std::make_shared<
                llgc::pattern::publisher::SubscriberServerGrpc<T>>(stream);
        std::shared_ptr<llgc::pattern::publisher::ConnectorSubscriberGrpc<T>>
            connector = std::make_shared<
                llgc::pattern::publisher::ConnectorSubscriberGrpc<T>>(
                subscriber, stream);
        // Ignore return value.
        // Can fail if SetOptionFailAlreadySubscribed is set.
        if (this->AddSubscriber(message.add_subscriber().id_message(),
                                connector))
        {
          // Empty body
        }
      }
      else if (enumeration == std::remove_reference<decltype(
                                  message)>::type::kRemoveSubscriber)
      {
        std::shared_ptr<llgc::pattern::publisher::SubscriberServerGrpc<T>>
            subscriber = std::make_shared<
                llgc::pattern::publisher::SubscriberServerGrpc<T>>(stream);
        std::shared_ptr<llgc::pattern::publisher::ConnectorSubscriberGrpc<T>>
            connector = std::make_shared<
                llgc::pattern::publisher::ConnectorSubscriberGrpc<T>>(
                subscriber, stream);
        // Don't assert if failed because it will stop server.
        BUGCONT(std::cout,
                this->RemoveSubscriber(message.remove_subscriber().id_message(),
                                       connector),
                grpc::Status::CANCELLED);
      }
    }

    BUGCONT(std::cout, this->Forward(messages), grpc::Status::CANCELLED);
  }

  return grpc::Status::OK;
}

template <typename T, typename S>
bool llgc::pattern::publisher::PublisherGrpc<T, S>::Listen()
{
  std::stringstream ss;

  ss << "0.0.0.0:" << this->GetPort();

  grpc::ServerBuilder builder_;

  builder_.AddListeningPort(ss.str(), grpc::InsecureServerCredentials());
  builder_.RegisterService(this);

  server_ = std::move(builder_.BuildAndStart());

  BUGCRIT(std::cout, server_ != nullptr, false,
          "Fail to start server at port " + std::to_string(this->GetPort()) +
              " for 0.0.0.0.\n");

  return true;
}

template <typename T, typename S>
bool llgc::pattern::publisher::PublisherGrpc<T, S>::Wait()
{
  std::thread t([this]() {
    if (server_ != nullptr)
    {
      server_->Wait();
    }
  });

  this->thread_wait_ = std::move(t);

  return true;
}

template <typename T, typename S>
void llgc::pattern::publisher::PublisherGrpc<T, S>::Stop()
{
  if (server_ != nullptr)
  {
    server_->Shutdown();
  }
}

/** \fn llgc::pattern::publisher::PublisherGrpc::PublisherGrpc(PublisherGrpc&& other)
 * \brief Delete move constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn llgc::pattern::publisher::PublisherGrpc::PublisherGrpc(PublisherGrpc const& other)
 * \brief Delete copy constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn PublisherGrpc& llgc::pattern::publisher::PublisherGrpc::operator=(PublisherGrpc&& other)
 * \brief Delete move operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn PublisherGrpc& llgc::pattern::publisher::PublisherGrpc::operator=(PublisherGrpc const& other)
 * \brief Delete copy operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \var llgc::pattern::publisher::PublisherGrpc::server_
 * \brief Server gRPC.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
