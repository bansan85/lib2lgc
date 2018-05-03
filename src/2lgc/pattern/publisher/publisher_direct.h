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

#ifndef PATTERN_PUBLISHER_PUBLISHER_DIRECT_H_
#define PATTERN_PUBLISHER_PUBLISHER_DIRECT_H_

#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/publisher.h>
#include <memory>

/**
 * @brief Namespace for the pattern publisher.
 */
namespace llgc::pattern::publisher
{
/**
 * @brief Server that will be used to managed subscribers and to keep and send
 *        messages.
 */
template <typename T>
class PublisherDirect
    : public Publisher<T, std::weak_ptr<ConnectorInterface<T>>>
{
 public:
  /**
   * @brief Default constructor.
   */
  PublisherDirect() = default;

  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  PublisherDirect(PublisherDirect&& other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  PublisherDirect(PublisherDirect const& other) = delete;

  /**
   * @brief Delete the move operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  PublisherDirect& operator=(PublisherDirect&& other) & = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  PublisherDirect& operator=(PublisherDirect const& other) & = delete;
};

}  // namespace llgc::pattern::publisher

#endif  // PATTERN_PUBLISHER_PUBLISHER_DIRECT_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
