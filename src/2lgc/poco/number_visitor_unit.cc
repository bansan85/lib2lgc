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

#include <2lgc/error/show.h>
#include <2lgc/math/compare_decimal.h>
#include <2lgc/poco/number.h>
#include <2lgc/poco/number.pb.h>
#include <2lgc/poco/number_impl.h>
#include <2lgc/poco/number_visitor_unit.h>
#include <2lgc/poco/number_visitor_value.h>
#include <2lgc/poco/raw.pb.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/util/message_differencer.h>
#include <memory>

bool llgc::poco::NumberVisitorUnit::Visit(const Number_Constant &data,
                                          std::string *return_value) const
{
  BUGPARAM(static_cast<void *>(return_value), return_value != nullptr, false);

  BUGLIB(data.Message().constant().unit().SerializeToString(return_value),
         false, "protobuf");

  return true;
}

bool llgc::poco::NumberVisitorUnit::Visit(const Number_NumOpNum &data,
                                          std::string *return_value) const
{
  BUGPARAM(static_cast<void *>(return_value), return_value != nullptr, false);

  msg::Number_Unit unit1;
  msg::Number_Unit unit2;
  msg::Number_Unit unit;
  std::string return_accept;
  BUGCONT(data.Number1()->Accept(*this, &return_accept), false);
  BUGLIB(unit1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(data.Number2()->Accept(*this, &return_accept), false);
  BUGLIB(unit2.ParseFromString(return_accept), false, "protobuf");

  switch (data.Message().number_op_number().operator_())
  {
    case msg::Number_Operator_PLUS:
    case msg::Number_Operator_MOINS:
    {
      BUGUSER(google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      BUGLIB(unit1.SerializeToString(return_value), false, "protobuf");
      return true;
    }
    case msg::Number_Operator_MULTIPLICATION:
    {
      BUGCONT(Unit::UnitOp(unit1, unit2, msg::Number_Operator_MULTIPLICATION,
                           &unit),
              false);
      BUGLIB(unit.SerializeToString(return_value), false, "protobuf");
      return true;
    }
    case msg::Number_Operator_DIVISION:
    {
      NumberVisitorVal visitor_val;

      BUGCONT(data.Number2()->Accept(visitor_val, &return_accept), false);
      msg::Double double_val;
      BUGLIB(double_val.ParseFromString(return_accept), false, "protobuf");
      BUGUSER(!llgc::math::Compare::AlmostEqualRelativeAndAbsD(
                  double_val.value(), 0., 1e-15, 1e-15),
              false, "Divide by zero.");
      BUGCONT(Unit::UnitOp(unit1, unit2, msg::Number_Operator_DIVISION, &unit),
              false);
      BUGLIB(unit.SerializeToString(return_value), false, "protobuf");
      return true;
    }
    case ::google::protobuf::kint32min:
    case ::google::protobuf::kint32max:
    default:
    {
      return false;
    }
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
