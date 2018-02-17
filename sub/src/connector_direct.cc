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

#include "connector_direct.h"

template <typename T>
bool pattern::publisher::ConnectorDirect<T>::Equals(
    const ConnectorInterface *connector) const {
  const ConnectorDirect<T> *connector_direct =
      dynamic_cast<const ConnectorDirect<T> *>(connector);

  if (connector_direct == nullptr) {
    return false;
  }

  return subscriber_->Equals(connector->GetSubscriber());
}

template <typename T>
bool pattern::publisher::ConnectorDirect<T>::AddSubscriber(
    uint32_t id_message, std::shared_ptr<ConnectorInterface> subscriber) {
  return server_->AddSubscriber(id_message, subscriber);
}

template <typename T>
void pattern::publisher::ConnectorDirect<T>::Send(
    const std::shared_ptr<const std::string> &message) {
  server_->Forward(message);
}

template <typename T>
void pattern::publisher::ConnectorDirect<T>::Listen(
    const std::shared_ptr<const std::string> &message) {
  subscriber_->Listen(message);
}

template <typename T>
bool pattern::publisher::ConnectorDirect<T>::RemoveSubscriber(
    uint32_t id_message, std::shared_ptr<ConnectorInterface> subscriber) {
  return server_->RemoveSubscriber(id_message, subscriber);
}
