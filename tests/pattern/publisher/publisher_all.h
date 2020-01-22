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

#ifndef PATTERN_PUBLISHER_PUBLISHER_ALL_H_
#define PATTERN_PUBLISHER_PUBLISHER_ALL_H_

#include <2lgc/compat.h>
#include <functional>
#include <thread>
#include <2lgc/pattern/publisher/subscriber_local.h>

namespace llgc::pattern::publisher::test
{
/**
 * @brief Simple implementation of a direct subscriber.
 */
template <typename T>
class Subscriber final : public llgc::pattern::publisher::SubscriberLocal<T>
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] id Id of the subscriber.
   */
  explicit Subscriber(uint32_t id)
      : SubscriberLocal<T>(id),
        value(0),
        message_vector(T::Msg::DataCase::kTest + 1)
  {
    message_vector[0] = nullptr;
    message_vector[T::Msg::DataCase::kTest] = &Subscriber::TestFct;
  }

  /**
   * @brief Receive message from publisher.
   *
   * @param[in] messages Message from the publisher in protobuf format.
   */
  bool Listen(const T& messages) override
  {
    for (int i = 0; i < messages.msg_size(); i++)
    {
      const typename T::Msg& message = messages.msg(i);

      assert(message_vector[message.data_case()] != nullptr);
      message_vector[message.data_case()](*this, message);
    }
    return true;
  }

  /**
   * @brief Function that will be run with test event.
   *
   * @param[in] message Action that contains the test event.
   */
  void TestFct(const typename T::Msg& message)
  {
    (void)message;
    value++;
  }

  /**
   * @brief value for test.
   */
  std::atomic<size_t> value;

  /**
   * @brief Function to execute.
   */
  std::vector<std::function<void(Subscriber&, const typename T::Msg&)>> message_vector;
};

class Publisher
{
 public:
  template <typename T, typename U, typename V>
  static void All(U* subscriber, V* server, size_t delay);

  static void WaitUpToTenSecond(const std::function<bool()>& test);
};
}

#ifndef TEMPLATE_CLASS
#include "publisher_all.cc"
#endif

#endif  // PATTERN_PUBLISHER_PUBLISHER_ALL_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
