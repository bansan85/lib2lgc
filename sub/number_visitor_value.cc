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

#include "number_visitor_value.h"

// Google protobuf library
#include <google/protobuf/util/message_differencer.h>

// lib2lgcPoco
#include <raw.pb.h>

// lib2lgcMath
#include <compare_decimal.h>

// macro
#include <handle_error.h>

// Current project
#include "number.h"
#include "number_visitor_unite.h"

bool pattern::visitor::NumberVisitorVal::Visit(
    const Number_Constant &data, std::string *return_value) const {
  BUGPARAM(return_value, "%p", return_value != nullptr, false);

  msg::Double val;
  val.set_value(data.message().constant().value());

  BUGLIB(val.SerializeToString(return_value), false, "protobuf");
  return true;
}

bool pattern::visitor::NumberVisitorVal::Visit(
    const Number_NumOpNum &data, std::string *return_value) const {
  BUGPARAM(return_value, "%p", return_value != nullptr, false);

  msg::Double val1;
  msg::Double val2;
  msg::Double val;
  std::string return_accept;
  BUGCONT(data.number1().Accept(*this, &return_accept), false);
  BUGLIB(val1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(data.number2().Accept(*this, &return_accept), false);
  BUGLIB(val2.ParseFromString(return_accept), false, "protobuf");

  NumberVisitorUnite visitor_unite;
  msg::Number_Unite unite1;
  msg::Number_Unite unite2;
  BUGCONT(data.number1().Accept(visitor_unite, &return_accept), false);
  BUGLIB(unite1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(data.number2().Accept(visitor_unite, &return_accept), false);
  BUGLIB(unite2.ParseFromString(return_accept), false, "protobuf");

  switch (data.message().number_op_number().operator_()) {
    case msg::Number_Operator_PLUS: {
      BUGUSER(
          google::protobuf::util::MessageDifferencer::Equals(unite1, unite2),
          false, "Incompatible unit.\n");
      val.set_value(val1.value() + val2.value());
      break;
    }
    case msg::Number_Operator_MOINS: {
      BUGUSER(
          google::protobuf::util::MessageDifferencer::Equals(unite1, unite2),
          false, "Incompatible unit.\n");
      val.set_value(val1.value() - val2.value());
      break;
    }
    case msg::Number_Operator_MULTIPLICATION: {
      val.set_value(val1.value() * val2.value());
      break;
    }
    case msg::Number_Operator_DIVISION: {
      BUGUSER(!math::AlmostEqualRelativeAndAbsD(val2.value(), 0., 1e-15, 1e-15),
              false, "Divide by zero.");
      val.set_value(val1.value() / val2.value());
      break;
    }
    case ::google::protobuf::kint32min:
    case ::google::protobuf::kint32max:
    default: { return false; }
  }

  BUGLIB(val.SerializeToString(return_value), false, "protobuf");
  return true;
}
