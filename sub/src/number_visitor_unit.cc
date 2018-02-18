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

#include "number_visitor_unit.h"

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

bool pattern::visitor::NumberVisitorUnit::Visit(
    const Number_Constant &data, std::string *return_value) const {
  BUGPARAM(return_value, "%p", return_value != nullptr, false);

  BUGLIB(data.message().constant().unit().SerializeToString(return_value),
         false, "protobuf");

  return true;
}

bool pattern::visitor::NumberVisitorUnit::Visit(
    const Number_NumOpNum &data, std::string *return_value) const {
  BUGPARAM(return_value, "%p", return_value != nullptr, false);

  msg::Number_Unit unit1;
  msg::Number_Unit unit2;
  msg::Number_Unit unit;
  std::string return_accept;
  BUGCONT(data.number1()->Accept(*this, &return_accept), false);
  BUGLIB(unit1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(data.number2()->Accept(*this, &return_accept), false);
  BUGLIB(unit2.ParseFromString(return_accept), false, "protobuf");

  switch (data.message().number_op_number().operator_()) {
    case msg::Number_Operator_PLUS:
    case msg::Number_Operator_MOINS: {
      BUGUSER(google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      BUGLIB(unit1.SerializeToString(return_value), false, "protobuf");
      return true;
    }
    case msg::Number_Operator_MULTIPLICATION: {
      BUGCONT(Unit::UnitOp(unit1, unit2, msg::Number_Operator_MULTIPLICATION,
                           &unit),
              false);
      BUGLIB(unit.SerializeToString(return_value), false, "protobuf");
      return true;
    }
    case msg::Number_Operator_DIVISION: {
      NumberVisitorVal visitor_val;

      BUGCONT(data.number2()->Accept(visitor_val, &return_accept), false);
      msg::Double double_val;
      BUGLIB(double_val.ParseFromString(return_accept), false, "protobuf");
      BUGUSER(!Math::AlmostEqualRelativeAndAbsD(double_val.value(), 0., 1e-15,
                                                1e-15),
              false, "Divide by zero.");
      BUGCONT(Unit::UnitOp(unit1, unit2, msg::Number_Operator_MULTIPLICATION,
                           &unit),
              false);
      BUGLIB(unit.SerializeToString(return_value), false, "protobuf");
      return true;
    }
    case ::google::protobuf::kint32min:
    case ::google::protobuf::kint32max:
    default: { return false; }
  }
}
