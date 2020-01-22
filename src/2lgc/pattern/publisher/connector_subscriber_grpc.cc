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

#ifndef PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_GRPC_CC_
#define PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_GRPC_CC_

#include <2lgc/compat.h>
#include <2lgc/error/show.h>
#include <2lgc/pattern/publisher/connector_subscriber.h>
#include <2lgc/pattern/publisher/connector_subscriber_grpc.h>
#include <iostream>

/** \class llgc::pattern::publisher::ConnectorSubscriberGrpc
 * \brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 * \tparam T The protobuf message.
 */

/** \brief Default constructor.
 * \param[in] subscriber Subscriber to communicate with client.
 * \param[in] stream Stream to communicate with client.
 */
template <typename T>
INLINE_TEMPLATE
llgc::pattern::publisher::ConnectorSubscriberGrpc<T>::ConnectorSubscriberGrpc(
    std::shared_ptr<SubscriberInterface<T>> subscriber,
    grpc_impl::ServerReaderWriter<T, T> *stream)
    : ConnectorSubscriber<T>(subscriber), stream_(stream)
{
}

/** \fn llgc::pattern::publisher::ConnectorSubscriberGrpc::~ConnectorSubscriberGrpc()
 * \brief Default virtual destructor.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorSubscriberGrpc::ConnectorSubscriberGrpc(ConnectorSubscriberGrpc &&other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::publisher::ConnectorSubscriberGrpc::ConnectorSubscriberGrpc(ConnectorSubscriberGrpc const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn ConnectorSubscriberGrpc & llgc::pattern::publisher::ConnectorSubscriberGrpc::operator=(ConnectorSubscriberGrpc &&other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn ConnectorSubscriberGrpc & llgc::pattern::publisher::ConnectorSubscriberGrpc::operator=(ConnectorSubscriberGrpc const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

template <typename T>
INLINE_TEMPLATE bool llgc::pattern::publisher::ConnectorSubscriberGrpc<T>::Send(
    const T &message)
{
  BUGLIB(std::cout, stream_->Write(message), false, "grpc");
  return true;
}

/** \var llgc::pattern::publisher::ConnectorSubscriberGrpc::stream_
 * \brief Link to the server.
 */

#endif  // PATTERN_PUBLISHER_CONNECTOR_SUBSCRIBER_GRPC_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
