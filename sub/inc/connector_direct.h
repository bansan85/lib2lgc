/* Copyright [2017] LE GARREC Vincent
 *
 * This file is part of 2LGC.
 *
 * 2LGC is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * 2LGC is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with 2LGC. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONNECTOR_DIRECT_H_
#define CONNECTOR_DIRECT_H_

#include "connector_interface.h"

// Current project
#include "publisher_base.h"

namespace pattern {
namespace publisher {

/**
 * @brief Interface that define functions that allow subscriber to communicate
 *        to server and server to subscriber.
 *
 * There's could be two kind of connector. First, direct connection, the other
 * one is connected throw TCP/IP.
 */
template <typename T>
class ConnectorDirect : public ConnectorInterface {
 public:
  explicit ConnectorDirect(
      const std::shared_ptr<SubscriberInterface> &subscriber,
      const std::shared_ptr<PublisherBase<T>> &server)
      : ConnectorInterface(subscriber), server_(server) {}

  bool Equals(const ConnectorInterface *connector) const override CHK;

  bool AddSubscriber(uint32_t id_message,
                     std::shared_ptr<ConnectorInterface> subscriber) CHK;

  /**
   * @brief Send message.
   *
   * @param id_message The id of the message.
   * @param data Data of the message in ProtoBuf, SerializeToString.
   */
  void Send(const std::shared_ptr<const std::string> &message) override CHK;
  void Listen(const std::shared_ptr<const std::string> &message) override CHK;

  bool RemoveSubscriber(
      uint32_t id_message,
      std::shared_ptr<ConnectorInterface> subscriber) override CHK;

  /**
   * @brief Default virtual destructor.
   */
  virtual ~ConnectorDirect() {}

 private:
  std::shared_ptr<PublisherBase<T>> server_;
};

}  // namespace publisher
}  // namespace pattern

#endif  // CONNECTOR_DIRECT_H_
