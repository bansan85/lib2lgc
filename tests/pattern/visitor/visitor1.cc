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
#include <2lgc/poco/math_number.pb.h>
#include <2lgc/poco/number_impl.h>
#include <2lgc/poco/number_visitor_value.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <memory>
#include <string>

int main(int /* argc */, char* /* argv */ [])  // NS
{
  google::protobuf::Arena arena;

  // Defining number 5 meters.
  auto unit = std::make_unique<llgc::protobuf::math::Number_Unit>();
  unit->set_m(1.);
  std::shared_ptr<llgc::poco::Number_Constant> nombre =
      std::make_shared<llgc::poco::Number_Constant>(1, 5.0, unit.release());
  llgc::poco::NumberVisitorVal visitor;

  // Check Serialization.
  std::string return_value;
  assert(nombre->Accept(visitor, &return_value));
  llgc::protobuf::math::Double double_value;
  assert(double_value.ParseFromString(return_value));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(double_value.value(), 5.,
                                                     1.e-10, 1));

  // Check 5 meters * 10.
  unit = std::make_unique<llgc::protobuf::math::Number_Unit>();
  std::shared_ptr<llgc::poco::Number_Constant> nombre2 =
      std::make_shared<llgc::poco::Number_Constant>(2, 10.0, unit.release());
  llgc::poco::Number_NumOpNum nombre3(
      3, nombre, llgc::protobuf::math::Number_Operator_MULTIPLICATION, nombre2);
  assert(nombre3.Accept(visitor, &return_value));
  assert(double_value.ParseFromString(return_value));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(double_value.value(), 50.,
                                                     1.e-10, 1));

  // Check 5 meters / 3.
  unit = std::make_unique<llgc::protobuf::math::Number_Unit>();
  std::shared_ptr<llgc::poco::Number_Constant> nombre4 =
      std::make_shared<llgc::poco::Number_Constant>(4, 3.0, unit.release());
  llgc::poco::Number_NumOpNum nombre5(
      5, nombre, llgc::protobuf::math::Number_Operator_DIVISION, nombre4);
  assert(nombre5.Accept(visitor, &return_value));
  assert(double_value.ParseFromString(return_value));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(double_value.value(),
                                                     5. / 3., 1.e-10, 1));

  /*
  llgc::protobuf::math::Double dd;
  dd.set_value(45.9);
  const google::protobuf::Descriptor * desc = dd.GetDescriptor();

  std::cout << desc->name() << std::endl;
  std::cout << dd.GetTypeName() << std::endl;

  std::fstream output("/tmp/test",
                      std::ios::out | std::ios::trunc | std::ios::binary);
  dd.SerializeToOstream(&output);
  */

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
