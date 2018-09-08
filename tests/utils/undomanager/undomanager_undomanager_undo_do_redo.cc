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

// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/error/show.h>
#include <2lgc/math/compare_decimal.h>
#include <2lgc/pattern/abstract_factory.h>
#include <2lgc/pattern/command/undo_interface.h>
#include <2lgc/utils/undomanager/undomanager.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <ext/alloc_traits.h>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "undo_command1.pb.h"
#include "undomanager_all_commands.h"

#ifdef TEMPLATE_CLASS
#include <2lgc/utils/tree.h>

#include <2lgc/pattern/abstract_factory.cc>
#include <2lgc/utils/tree.cc>
#include <2lgc/utils/undomanager/undomanager.cc>

template class llgc::pattern::AbstractFactory<
    llgc::protobuf::test::UndoManager1, llgc::pattern::command::UndoInterface>;
template class llgc::utils::undomanager::Undomanager<
    llgc::protobuf::test::UndoManager1, llgc::pattern::command::UndoInterface>;
template class llgc::utils::Tree<llgc::pattern::command::UndoInterface>;
#else
namespace llgc::utils
{
template <typename T>
class Tree;
}
#endif

/**
 * @brief Example of factory.
 */
class Factory : public llgc::pattern::AbstractFactory<
                    llgc::protobuf::test::UndoManager1,
                    llgc::pattern::command::UndoInterface>
{
 public:
  explicit Factory(double* number_ref)
      : AbstractFactory(llgc::protobuf::test::UndoManager1::kFail + 1),
        number_ref_(number_ref)
  {
    map_factory_[llgc::protobuf::test::UndoManager1::kAdd] = [this](auto x) {
      return this->Add(x);
    };
    map_factory_[llgc::protobuf::test::UndoManager1::kFail] = [this](auto x) {
      return this->Fail(x);
    };
  }

 private:
  double* number_ref_;

  std::unique_ptr<llgc::pattern::command::UndoInterface> Add(
      const llgc::protobuf::test::UndoManager1& message)
  {
    assert(message.has_add());

    auto retval =
        std::make_unique<CommandAdd>(message.add().number(), number_ref_);

    return std::move(retval);
  }

  std::unique_ptr<llgc::pattern::command::UndoInterface> Fail(
      const llgc::protobuf::test::UndoManager1& message)
  {
    assert(message.has_fail());

    auto retval = std::make_unique<CommandFail>();

    return std::move(retval);
  }
};

int main(int /* argc */, char* /* argv */ [])  // NS
{
  double number = 0.;
  auto factory = std::make_unique<Factory>(&number);
  llgc::utils::undomanager::Undomanager<llgc::protobuf::test::UndoManager1,
                                        llgc::pattern::command::UndoInterface>
      undomanager(std::move(factory));

  EXECUTE_AND_ABORT(std::cout, undomanager.AddCommand("") == nullptr);
  EXECUTE_AND_ABORT(std::cout, undomanager.AddCommand("tatayoyo") == nullptr);

  std::string message_str;
  llgc::protobuf::test::UndoManager1 undo;
  auto add = std::make_unique<llgc::protobuf::test::UndoManager1_Add>();
  add->set_number(4.0);
  undo.set_allocated_add(add.release());
  EXECUTE_AND_ABORT(std::cout, undo.SerializeToString(&message_str));

  // Id 0
  llgc::utils::Tree<llgc::pattern::command::UndoInterface>* undo2 =
      undomanager.AddCommand(message_str);
  assert(undo2 != nullptr);
  EXECUTE_AND_ABORT(std::cout, undomanager.DoCommand(0));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(number, 4., 1.e-10, 4));
  EXECUTE_AND_ABORT(std::cout, !undomanager.DoCommand(0));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(number, 4., 1.e-10, 4));

  // Id 1
  undo2 = undomanager.AddCommand(0, message_str);
  assert(undo2 != nullptr);
  EXECUTE_AND_ABORT(std::cout, undomanager.DoCommand(1));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(number, 8., 1.e-10, 4));
  EXECUTE_AND_ABORT(std::cout, !undomanager.DoCommand(1));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(number, 8., 1.e-10, 4));

  EXECUTE_AND_ABORT(std::cout, undomanager.UndoCommand(1));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(number, 4., 1.e-10, 4));
  EXECUTE_AND_ABORT(std::cout, undomanager.UndoCommand(0));
  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsD(number, 0., 1.e-13,
                                                         1.e-13));

  EXECUTE_AND_ABORT(std::cout, undomanager.DoCommands(0, 1));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(number, 8., 1.e-10, 4));
  EXECUTE_AND_ABORT(std::cout, !undomanager.DoCommands(0, 1));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(number, 8., 1.e-10, 4));

  EXECUTE_AND_ABORT(std::cout, undomanager.UndoCommands(1, 0));
  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsD(number, 0., 1.e-13,
                                                         1.e-13));
  EXECUTE_AND_ABORT(std::cout, !undomanager.UndoCommands(1, 0));
  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsD(number, 0., 1.e-13,
                                                         1.e-13));

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
