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

#include <2lgc/pattern/abstract_factory.h>
#include <abstract_factory.pb.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <ext/alloc_traits.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Interface of a command.
 */
class CommonCommand
{
 public:
  CommonCommand() = default;
  virtual ~CommonCommand() = default;

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  CommonCommand(CommonCommand&& other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  CommonCommand(CommonCommand const& other) = delete;

  /**
   * @brief Delete the move operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  CommonCommand& operator=(CommonCommand&& other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  CommonCommand& operator=(CommonCommand const& other) = delete;
#endif  // !SWIG

  virtual void Modify() = 0;
  virtual bool Check() = 0;
};

#ifdef TEMPLATE_CLASS
#include <2lgc/config.h>

#include <2lgc/pattern/abstract_factory.cc>

template class llgc::pattern::AbstractFactory<
    llgc::protobuf::test::AbstractFactoryMsg, CommonCommand>;
#endif

/**
 * @brief Example of implementation of command.
 */
class CommandTest : public CommonCommand
{
 public:
  CommandTest() = default;
  ~CommandTest() override = default;

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  CommandTest(CommandTest&& other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  CommandTest(CommandTest const& other) = delete;

  /**
   * @brief Delete the move operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  CommandTest& operator=(CommandTest&& other) = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  CommandTest& operator=(CommandTest const& other) = delete;
#endif  // !SWIG

  void Modify() override { member_ = true; }

  bool Check() override { return member_; }

 private:
  bool member_{false};
};

/**
 * @brief Example of factory.
 */
class Factory : public llgc::pattern::AbstractFactory<
                    llgc::protobuf::test::AbstractFactoryMsg, CommonCommand>
{
 public:
  Factory()
      : AbstractFactory(llgc::protobuf::test::AbstractFactoryMsg::kTest + 1)
  {
    map_factory_[llgc::protobuf::test::AbstractFactoryMsg::kTest] =
        [this](auto x) { return this->GetTest(x); };
  }

 private:
  std::unique_ptr<CommonCommand> GetTest(
      const llgc::protobuf::test::AbstractFactoryMsg& message)
  {
    assert(message.has_test());

    auto retval = std::make_unique<CommandTest>();

    return std::move(retval);
  }
};

int main(int /* argc */, char* /* argv */ [])  // NS
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  Factory factory;

  llgc::protobuf::test::AbstractFactoryMsg message;
  auto add = std::make_unique<llgc::protobuf::test::AbstractFactoryMsg_Test>();
  message.set_allocated_test(add.release());

  std::string message_in_string;
  message.SerializeToString(&message_in_string);

  std::unique_ptr<CommonCommand> command = factory.Create(message_in_string);

  assert(command != nullptr);
  assert(!command->Check());
  command->Modify();
  assert(command->Check());

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
