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

#include "publisher_all.h"

// T Type of protobuf message
// U Type of subscriber
// V Type of the server
template <typename T, typename U, typename V>
void llgc::pattern::publisher::test::Publisher::All(U* subscriber, V* server,
                                                    size_t delay)
{
  assert(subscriber->AddSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));

  assert(subscriber->value == 0);

  // Check first message.
  T messages;
  auto message = messages.add_msg();
  auto message_test = std::make_unique<typename T::Msg::Test>();
  message->set_allocated_test(message_test.release());
  assert(subscriber->Send(messages));
  WaitUpToTenSecond([&subscriber]() { return subscriber->value == 1; });

  // Test lock forward.
  subscriber->value = 0;
  {
    llgc::utils::thread::CountLock<size_t> lock = server->LockForward();
    assert(subscriber->Send(messages));
    // Wait one second to be sure that the message is not send.
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    assert(subscriber->value == 0);
  }
  WaitUpToTenSecond([&subscriber]() { return subscriber->value == 1; });

  // Remove the first subscriber.
  subscriber->value = 0;
  assert(subscriber->RemoveSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->Send(messages));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->value == 0);

  // Double insert
  assert(subscriber->AddSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->AddSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->Send(messages));
  WaitUpToTenSecond([&subscriber]() { return subscriber->value == 2; });
  assert(subscriber->RemoveSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->Send(messages));
  WaitUpToTenSecond([&subscriber]() { return subscriber->value == 3; });
  assert(subscriber->RemoveSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->Send(messages));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->value == 3);
  assert(!server->GetOptionFailAlreadySubscribed());
  server->SetOptionFailAlreadySubscribed(true);
  assert(subscriber->AddSubscriber(T::Msg::DataCase::kTest));
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));

  // Here, AddSubscriber will not failed because the TCP server can't return a
  // value.
  (void)!subscriber->AddSubscriber(T::Msg::DataCase::kTest);
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  assert(subscriber->Send(messages));
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
