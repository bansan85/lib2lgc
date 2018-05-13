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

#ifndef PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV4_H_
#define PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV4_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>
#include <cstdint>
#include <memory>
#include <string>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
template <typename T>
class SubscriberInterface;

/**
 * @brief IPV4 of ConnectorPublisherTcp.
 */
template <typename T>
class ConnectorPublisherTcpIpv4 : public ConnectorPublisherTcp<T>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] subscriber The subscriber.
   * @param[in] ip The IP of the server.
   * @param[in] port The port of the server.
   */
  ConnectorPublisherTcpIpv4(std::shared_ptr<SubscriberInterface<T>> subscriber,
                            const std::string ip, uint16_t port);

  /**
   * @brief Default virtual destructor.
   */
  virtual ~ConnectorPublisherTcpIpv4();

#ifndef SWIG
  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorPublisherTcpIpv4(ConnectorPublisherTcpIpv4 &&other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  ConnectorPublisherTcpIpv4(ConnectorPublisherTcpIpv4 const &other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorPublisherTcpIpv4 &operator=(ConnectorPublisherTcpIpv4 &&other) =
      delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  ConnectorPublisherTcpIpv4 &operator=(
      ConnectorPublisherTcpIpv4 const &other) & = delete;
#endif  // !SWIG

 protected:
  /**
   * @brief Start connection with server.
   *
   * @return true if no problem.
   */
  bool Connect() CHK;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_CONNECTOR_PUBLISHER_TCP_IPV4_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
