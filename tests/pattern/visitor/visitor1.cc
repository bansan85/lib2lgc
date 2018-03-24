// NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access2
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

#include <2lgc/math/compare_decimal.h>
#include <2lgc/poco/number.pb.h>
#include <2lgc/poco/number_impl.h>
#include <2lgc/poco/number_visitor_value.h>
#include <2lgc/poco/raw.pb.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <memory>
#include <string>

int main(int /* argc */, char* /* argv */ [])
{
  google::protobuf::Arena arena;

  // Defining number 5 meters.
  auto unit = std::make_unique<msg::Number_Unit>();
  unit->set_m(1.);
  std::shared_ptr<pattern::visitor::Number_Constant> nombre =
      std::make_shared<pattern::visitor::Number_Constant>(1, 5.0,
                                                          unit.release());
  pattern::visitor::NumberVisitorVal visitor;

  // Check Serialization.
  std::string return_value;
  assert(nombre->Accept(visitor, &return_value));
  msg::Double double_value;
  assert(double_value.ParseFromString(return_value));
  assert(Math::AlmostEqualUlpsAndAbsD(double_value.value(), 5., 1.e-10, 1));

  // Check 5 meters * 10.
  unit = std::make_unique<msg::Number_Unit>();
  std::shared_ptr<pattern::visitor::Number_Constant> nombre2 =
      std::make_shared<pattern::visitor::Number_Constant>(2, 10.0,
                                                          unit.release());
  pattern::visitor::Number_NumOpNum nombre3(
      3, nombre, msg::Number_Operator_MULTIPLICATION, nombre2);
  assert(nombre3.Accept(visitor, &return_value));
  assert(double_value.ParseFromString(return_value));
  assert(Math::AlmostEqualUlpsAndAbsD(double_value.value(), 50., 1.e-10, 1));

  // Check 5 meters / 3.
  unit = std::make_unique<msg::Number_Unit>();
  std::shared_ptr<pattern::visitor::Number_Constant> nombre4 =
      std::make_shared<pattern::visitor::Number_Constant>(4, 3.0,
                                                          unit.release());
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

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
