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

#ifndef TESTS_PATTERN_PUBLISHER_PUBLISHER_ALL_CC_
#define TESTS_PATTERN_PUBLISHER_PUBLISHER_ALL_CC_

#include "publisher_all.h"
#include <2lgc/error/show.h>

// T Type of protobuf message
// U Type of subscriber
// V Type of the server
template <typename T, typename U, typename V>
INLINE_TEMPLATE void llgc::pattern::publisher::test::Publisher::All(
    U* subscriber, V* server, size_t delay)
{
  EXECUTE_AND_ABORT(std::cout,
                    subscriber->AddSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));

  assert(subscriber->value == 0);

  // Check first message.
  T messages;
  auto message = messages.add_msg();
  auto message_test = std::make_unique<typename T::Msg::Test>();
  message->set_allocated_test(message_test.release());
  EXECUTE_AND_ABORT(std::cout, subscriber->Send(messages));
  WaitUpToTenSecond([&subscriber]() { return subscriber->value == 1; });

  // Test lock forward.
  subscriber->value = 0;
  {
    llgc::utils::thread::CountLock<size_t> lock = server->LockForward();
    EXECUTE_AND_ABORT(std::cout, subscriber->Send(messages));
    // Wait one second to be sure that the message is not send.
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    assert(subscriber->value == 0);
  }
  WaitUpToTenSecond([&subscriber]() { return subscriber->value == 1; });

  // Remove the first subscriber.
  subscriber->value = 0;
  EXECUTE_AND_ABORT(std::cout,
                    subscriber->RemoveSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  EXECUTE_AND_ABORT(std::cout, subscriber->Send(messages));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->value == 0);

  // Double insert
  EXECUTE_AND_ABORT(std::cout,
                    subscriber->AddSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  EXECUTE_AND_ABORT(std::cout,
                    subscriber->AddSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  EXECUTE_AND_ABORT(std::cout, subscriber->Send(messages));
  WaitUpToTenSecond([&subscriber]() { return subscriber->value == 2; });
  EXECUTE_AND_ABORT(std::cout,
                    subscriber->RemoveSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  EXECUTE_AND_ABORT(std::cout, subscriber->Send(messages));
  WaitUpToTenSecond([&subscriber]() { return subscriber->value == 3; });
  EXECUTE_AND_ABORT(std::cout,
                    subscriber->RemoveSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  EXECUTE_AND_ABORT(std::cout, subscriber->Send(messages));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->value == 3);
  assert(!server->GetOptionFailAlreadySubscribed());
  server->SetOptionFailAlreadySubscribed(true);
  EXECUTE_AND_ABORT(std::cout,
                    subscriber->AddSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));

  // Here, AddSubscriber will not failed because the TCP server can't return a
  // value.
  (void)!subscriber->AddSubscriber(T::Msg::DataCase::kTest);
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  EXECUTE_AND_ABORT(std::cout, subscriber->Send(messages));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->value == 4);
}

void llgc::pattern::publisher::test::Publisher::WaitUpToTenSecond(
    const std::function<bool()>& test)
{
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  do
  {
    end = std::chrono::system_clock::now();
    assert(
        static_cast<size_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count()) < 10000);
  } while (!test());
}

#endif  // TESTS_PATTERN_PUBLISHER_PUBLISHER_ALL_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
