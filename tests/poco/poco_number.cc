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
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <memory>

int main(int /* argc */, char * /* argv */ [])  // NS
{
  auto unit = std::make_unique<llgc::protobuf::math::Number_Unit>();
  unit->set_m(1.);
  std::shared_ptr<llgc::poco::Number_Constant> constant1 =
      std::make_shared<llgc::poco::Number_Constant>(1, 0.5, unit.release());

  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(constant1->GetVal(), 0.5,
                                                     0.5e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(constant1->GetVal(), 0.5,
                                                     0.5e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsF(constant1->GetUnit().m(),
                                                     1.f, 1.e-8f, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsF(constant1->GetUnit().m(),
                                                     1.f, 1.e-8f, 4));

  unit = std::make_unique<llgc::protobuf::math::Number_Unit>();
  unit->set_m(1.);
  std::shared_ptr<llgc::poco::Number_Constant> constant2 =
      std::make_shared<llgc::poco::Number_Constant>(2, 2.3, unit.release());

  llgc::poco::Number_NumOpNum addition(
      3, constant1, llgc::protobuf::math::Number_Operator_PLUS, constant2);
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(addition.GetVal(), 2.8,
                                                     2.8e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(addition.GetVal(), 2.8,
                                                     2.8e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsF(addition.GetUnit().m(),
                                                     1.f, 1.e-8f, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsF(addition.GetUnit().m(),
                                                     1.f, 1.e-8f, 4));
  llgc::poco::Number_NumOpNum moins(
      4, constant1, llgc::protobuf::math::Number_Operator_MOINS, constant2);
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(moins.GetVal(), -1.8,
                                                     1.8e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsF(moins.GetUnit().m(), 1.f,
                                                     1.e-13f, 4));
  llgc::poco::Number_NumOpNum multiplication(
      5, constant1, llgc::protobuf::math::Number_Operator_MULTIPLICATION,
      constant2);
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(multiplication.GetVal(),
                                                     1.15, 1.4e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsF(
      multiplication.GetUnit().m(), 2.f, 2.e-13f, 4));
  llgc::poco::Number_NumOpNum division(
      6, constant1, llgc::protobuf::math::Number_Operator_DIVISION, constant2);
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(division.GetVal(),
                                                     0.5 / 2.3, 0.18e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsF(division.GetUnit().m(),
                                                     0.f, 1.e-13f, 4));

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
