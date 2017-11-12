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

#include "number_visitor_unite.h"

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
#include "number_visitor_value.h"

bool pattern::visitor::NumberVisitorUnite::Visit(
    const Number_Constant &data, std::string *return_value) const {
  BUGPARAM(return_value, "%p", return_value != nullptr, false);

  BUGLIB(data.message().constant().unite().SerializeToString(return_value),
         false, "protobuf");

  return true;
}

bool pattern::visitor::NumberVisitorUnite::Visit(
    const Number_NumOpNum &data, std::string *return_value) const {
  BUGPARAM(return_value, "%p", return_value != nullptr, false);

  msg::Number_Unite unite1;
  msg::Number_Unite unite2;
  msg::Number_Unite unite;
  std::string return_accept;
  BUGCONT(data.number1().Accept(*this, &return_accept), false);
  BUGLIB(unite1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(data.number2().Accept(*this, &return_accept), false);
  BUGLIB(unite2.ParseFromString(return_accept), false, "protobuf");

  switch (data.message().number_op_number().operator_()) {
    case msg::Number_Operator_PLUS:
    case msg::Number_Operator_MOINS: {
      BUGUSER(
          google::protobuf::util::MessageDifferencer::Equals(unite1, unite2),
          false, "Incompatible unit.\n");
      BUGLIB(unite1.SerializeToString(return_value), false, "protobuf");
      return true;
    }
    case msg::Number_Operator_MULTIPLICATION: {
      BUGCONT(Unite::UniteOp(unite1, unite2,
                             msg::Number_Operator_MULTIPLICATION, &unite),
              false);
      BUGLIB(unite.SerializeToString(return_value), false, "protobuf");
      return true;
    }
    case msg::Number_Operator_DIVISION: {
      NumberVisitorVal visitor_val;

      BUGCONT(data.number2().Accept(visitor_val, &return_accept), false);
      msg::Double double_val;
      BUGLIB(double_val.ParseFromString(return_accept), false, "protobuf");
      BUGUSER(!math::AlmostEqualRelativeAndAbsD(double_val.value(), 0., 1e-15,
                                                1e-15),
              false, "Divide by zero.");
      BUGCONT(Unite::UniteOp(unite1, unite2,
                             msg::Number_Operator_MULTIPLICATION, &unite),
              false);
      BUGLIB(unite.SerializeToString(return_value), false, "protobuf");
      return true;
    }
    case ::google::protobuf::kint32min:
    case ::google::protobuf::kint32max:
    default: { return false; }
  }
}
