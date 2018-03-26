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

#include <2lgc/error/show.h>
#include <2lgc/pattern/publisher/publisher_base.h>
#include <memory>

template <typename M>
llgc::pattern::publisher::PublisherBase<M>::PublisherBase() = default;

template <typename M>
llgc::pattern::publisher::PublisherBase<M>::~PublisherBase() = default;

template <typename M>
void llgc::pattern::publisher::PublisherBase<M>::Forward(
    const std::shared_ptr<const std::string> &message)
{
  M actions;

  BUGUSER(actions.ParseFromString(*message.get()), ,
          "Failed to decode message.\n");

  for (int i = 0; i < actions.action_size(); i++)
  {
    // Must use auto because we don't know if M is in namespace or not.
    auto action = actions.action(i);

    auto iterpair = subscribers_.equal_range(action.data_case());

    auto it = iterpair.first;
    for (; it != iterpair.second; ++it)
    {
      it->second->Listen(message);
    }
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
