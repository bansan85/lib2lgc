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
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <memory>

int main(int /* argc */, char* /* argv */ [])  // NS
{
  std::unique_ptr<msg::Number_Unit> unit = std::make_unique<msg::Number_Unit>();
  unit->set_m(1.);
  std::shared_ptr<llgc::poco::Number_Constant> constant1 =
      std::make_shared<llgc::poco::Number_Constant>(1, 0.5, unit.release());

  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(constant1->GetVal(), 0.5,
                                                     0.5e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(constant1->GetVal(), 0.5,
                                                     0.5e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(constant1->GetUnit().m(),
                                                     1., 1e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(constant1->GetUnit().m(),
                                                     1., 1e-13, 4));

  unit = std::make_unique<msg::Number_Unit>();
  unit->set_m(1.);
  std::shared_ptr<llgc::poco::Number_Constant> constant2 =
      std::make_shared<llgc::poco::Number_Constant>(2, 2.3, unit.release());

  llgc::poco::Number_NumOpNum addition(3, constant1, msg::Number_Operator_PLUS,
                                       constant2);
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(addition.GetVal(), 2.8,
                                                     2.8e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(addition.GetVal(), 2.8,
                                                     2.8e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(addition.GetUnit().m(), 1.,
                                                     1e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(addition.GetUnit().m(), 1.,
                                                     1e-13, 4));
  llgc::poco::Number_NumOpNum moins(4, constant1, msg::Number_Operator_MOINS,
                                    constant2);
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(moins.GetVal(), -1.8,
                                                     1.8e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(moins.GetUnit().m(), 1.,
                                                     1e-13, 4));
  llgc::poco::Number_NumOpNum multiplication(
      5, constant1, msg::Number_Operator_MULTIPLICATION, constant2);
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(multiplication.GetVal(),
                                                     1.15, 1.4e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(
      multiplication.GetUnit().m(), 2., 2e-13, 4));
  llgc::poco::Number_NumOpNum division(
      6, constant1, msg::Number_Operator_DIVISION, constant2);
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(division.GetVal(),
                                                     0.5 / 2.3, 0.18e-13, 4));
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(division.GetUnit().m(), 0.,
                                                     1e-13, 4));

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
