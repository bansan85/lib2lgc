/* Copyright [2017] LE GARREC Vincent
 *
 * This file is part of 2LGC.
 *
 * 2LGC is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * 2LGC is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with 2LGC. If not, see <http://www.gnu.org/licenses/>.
 */

// C++ system
#include <fstream>
#include <iostream>
#include <string>

// Google protobuf library
#include <google/protobuf/arena.h>

// lib2lgcPocoGetSet
#include <number.h>
#include <number_visitor_value.h>

// lib2lgcPoco
#include <raw.pb.h>

// lib2lgcMath
#include <compare_decimal.h>

int main(int /* argc */, char * /* argv */ []) {
  google::protobuf::Arena arena;

  // Defining number 5 meters.
  msg::Number_Unit *unit = new msg::Number_Unit();
  unit->set_m(1.);
  std::shared_ptr<pattern::visitor::Number_Constant> nombre =
      std::make_shared<pattern::visitor::Number_Constant>(1, 5.0, unit);
  pattern::visitor::NumberVisitorVal visitor;

  // Check Serialization.
  std::string return_value;
  assert(nombre->Accept(visitor, &return_value));
  msg::Double double_value;
  assert(double_value.ParseFromString(return_value));
  assert(Math::AlmostEqualUlpsAndAbsD(double_value.value(), 5., 1.e-10, 1));

  // Check 5 meters * 10.
  unit = new msg::Number_Unit();
  std::shared_ptr<pattern::visitor::Number_Constant> nombre2 =
      std::make_shared<pattern::visitor::Number_Constant>(2, 10.0, unit);
  pattern::visitor::Number_NumOpNum nombre3(
      3, nombre, msg::Number_Operator_MULTIPLICATION, nombre2);
  assert(nombre3.Accept(visitor, &return_value));
  assert(double_value.ParseFromString(return_value));
  assert(Math::AlmostEqualUlpsAndAbsD(double_value.value(), 50., 1.e-10, 1));

  // Check 5 meters / 3.
  unit = new msg::Number_Unit();
  std::shared_ptr<pattern::visitor::Number_Constant> nombre4 =
      std::make_shared<pattern::visitor::Number_Constant>(4, 3.0, unit);
  pattern::visitor::Number_NumOpNum nombre5(
      5, nombre, msg::Number_Operator_DIVISION, nombre4);
  assert(nombre5.Accept(visitor, &return_value));
  assert(double_value.ParseFromString(return_value));
  assert(
      Math::AlmostEqualUlpsAndAbsD(double_value.value(), 5. / 3., 1.e-10, 1));

  /*
  msg::Double dd;
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
