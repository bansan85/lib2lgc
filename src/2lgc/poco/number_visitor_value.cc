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

bool llgc::poco::NumberVisitorVal::Visit(const Number_Constant &data,
                                         std::string *return_value) const
{
  BUGPARAM(static_cast<void *>(return_value), "%p", return_value != nullptr,
           false);

  msg::Double val;
  val.set_value(data.Message().constant().value());

  BUGLIB(val.SerializeToString(return_value), false, "protobuf");
  return true;
}

bool llgc::poco::NumberVisitorVal::Visit(const Number_NumOpNum &data,
                                         std::string *return_value) const
{
  BUGPARAM(static_cast<void *>(return_value), "%p", return_value != nullptr,
           false);

  msg::Double val1;
  msg::Double val2;
  msg::Double val;
  std::string return_accept;
  BUGCONT(data.Number1()->Accept(*this, &return_accept), false);
  BUGLIB(val1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(data.Number2()->Accept(*this, &return_accept), false);
  BUGLIB(val2.ParseFromString(return_accept), false, "protobuf");

  NumberVisitorUnit visitor_unit;
  msg::Number_Unit unit1;
  msg::Number_Unit unit2;
  BUGCONT(data.Number1()->Accept(visitor_unit, &return_accept), false);
  BUGLIB(unit1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(data.Number2()->Accept(visitor_unit, &return_accept), false);
  BUGLIB(unit2.ParseFromString(return_accept), false, "protobuf");

  switch (data.Message().number_op_number().operator_())
  {
    case msg::Number_Operator_PLUS:
    {
      BUGUSER(google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      val.set_value(val1.value() + val2.value());
      break;
    }
    case msg::Number_Operator_MOINS:
    {
      BUGUSER(google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      val.set_value(val1.value() - val2.value());
      break;
    }
    case msg::Number_Operator_MULTIPLICATION:
    {
      val.set_value(val1.value() * val2.value());
      break;
    }
    case msg::Number_Operator_DIVISION:
    {
      BUGUSER(!llgc::math::Compare::AlmostEqualRelativeAndAbsD(val2.value(), 0.,
                                                               1e-15, 1e-15),
              false, "Divide by zero.");
      val.set_value(val1.value() / val2.value());
      break;
    }
    case ::google::protobuf::kint32min:
    case ::google::protobuf::kint32max:
    default:
    {
      return false;
    }
  }

  BUGLIB(val.SerializeToString(return_value), false, "protobuf");
  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
