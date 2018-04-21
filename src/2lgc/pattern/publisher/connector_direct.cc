/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <2lgc/pattern/publisher/connector_direct.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/subscriber_interface.h>
#include <algorithm>
#include <memory>

template <typename T>
llgc::pattern::publisher::ConnectorDirect<T>::ConnectorDirect(
    std::shared_ptr<SubscriberInterface> subscriber,
    std::shared_ptr<PublisherBase<T>> server)
    : ConnectorInterface(std::move(subscriber)), server_(std::move(server))
{
}

template <typename T>
llgc::pattern::publisher::ConnectorDirect<T>::~ConnectorDirect() = default;

template <typename T>
bool llgc::pattern::publisher::ConnectorDirect<T>::Equals(
    const ConnectorInterface *connector) const
{
  auto connector_direct = dynamic_cast<const ConnectorDirect<T> *>(connector);

  if (connector_direct == nullptr)
  {
    return false;
  }

  return subscriber_->Equals(connector->GetSubscriber());
}

template <typename T>
bool llgc::pattern::publisher::ConnectorDirect<T>::AddSubscriber(
    uint32_t id_message)
{
  return server_->AddSubscriber(id_message, this->shared_from_this());
}

template <typename T>
void llgc::pattern::publisher::ConnectorDirect<T>::Send(
    std::shared_ptr<const std::string> message)
{
  server_->Forward(std::move(message));
}

template <typename T>
bool llgc::pattern::publisher::ConnectorDirect<T>::RemoveSubscriber(
    uint32_t id_message)
{
  return server_->RemoveSubscriber(id_message, this->shared_from_this());
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
