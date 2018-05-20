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

#include <2lgc/math/compare_decimal.h>
#include <2lgc/pattern/command/undo_interface.h>
#include <2lgc/text/printf.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include "undo_command1.pb.h"

/**
 * @brief Undo command that add float number.
 */
class CommandAdd : public llgc::pattern::command::UndoInterface
{
 public:
  CommandAdd(double number, double* number_ref)
      : number_(number), number_ref_(number_ref)
  {
  }

  ~CommandAdd() override = default;

  CommandAdd(CommandAdd && other) = delete;
  CommandAdd(CommandAdd const& other) = delete;
  CommandAdd& operator=(CommandAdd && other) & = delete;
  CommandAdd& operator=(CommandAdd const& other) & = delete;

  bool Do() override
  {
    if (done)
    {
      return false;
    }

    *number_ref_ += number_;

    done = true;

    return true;
  }

  bool Undo() override
  {
    if (!done)
    {
      return false;
    }

    *number_ref_ -= number_;

    done = false;

    return true;
  }

  std::unique_ptr<UndoInterface> Redo() override
  {
    auto retval = std::make_unique<CommandAdd>(number_, number_ref_);

    return std::move(retval);
  }

  bool ChangeModel() override { return true; }

  std::string GetDescription() const override
  {
    std::stringstream ss;

    llgc::text::Print::F(ss, "Add %", number_);

    return ss.str();
  }

  std::string GetPosition() const override { return std::string(); }

  size_t GetUser() const override { return 0; }

  std::string ToProtobuf() const override
  {
    llgc::protobuf::test::UndoManager1_Add add;
    add.set_number(number_);

    std::string command;
    if (!add.SerializeToString(&command))
    {
      return std::string();
    }
    return command;
  }

 private:
  const double number_;
  double* number_ref_;
};

int main(int /* argc */, char* /* argv */ [])  // NS
{
  double number_test = 0.;
  CommandAdd add4(4., &number_test);

  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsD(number_test, 0.,
                                                         1.e-13, 1.e-13));
  assert(add4.Do());
  assert(
      llgc::math::Compare::AlmostEqualUlpsAndAbsD(number_test, 4., 1.e-10, 4));
  assert(!add4.Do());
  assert(
      llgc::math::Compare::AlmostEqualUlpsAndAbsD(number_test, 4., 1.e-10, 4));
  assert(add4.Undo());
  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsD(number_test, 0.,
                                                         1.e-13, 1.e-13));
  assert(!add4.Undo());
  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsD(number_test, 0.,
                                                         1.e-13, 1.e-13));
  assert(add4.Do());
  assert(
      llgc::math::Compare::AlmostEqualUlpsAndAbsD(number_test, 4., 1.e-10, 4));
  assert(!add4.Do());
  auto add4bis = add4.Redo();
  assert(add4bis->Do());
  assert(
      llgc::math::Compare::AlmostEqualUlpsAndAbsD(number_test, 8., 1.e-10, 4));
  assert(!add4bis->Do());

  std::string add4serialize = add4.ToProtobuf();
  llgc::protobuf::test::UndoManager1_Add add_protobuf;

  assert(add_protobuf.ParseFromString(add4serialize));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(add_protobuf.number(), 4.,
                                                     1.e-10, 4));

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
