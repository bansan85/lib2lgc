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

#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/subscriber_interface.h>  // IWYU pragma: keep

template <typename T>
llgc::pattern::publisher::ConnectorInterface<T>::ConnectorInterface(
    std::shared_ptr<SubscriberInterface<T>> subscriber)
    : messages_(), next_id_(0), subscriber_(subscriber)
{
}

template <typename T>
void llgc::pattern::publisher::ConnectorInterface<T>::Listen(const T& message,
                                                             const bool hold)
{
  if (hold)
  {
    messages_.push(message);
  }
  else
  {
    subscriber_->Listen(message);
  }
}

template <typename T>
void llgc::pattern::publisher::ConnectorInterface<T>::ListenPending()
{
  while (!messages_.empty())
  {
    const T& it = messages_.front();
    subscriber_->Listen(it);
    messages_.pop();
  }
}
