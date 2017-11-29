
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

#include "config.h"

#include "number.h"

// C++ system
#include <cmath>

// Google protobuf library
#include <google/protobuf/util/message_differencer.h>

// lib2lgcPoco
#include <raw.pb.h>

// macros
#include <handle_error.h>

// Static variables
pattern::visitor::NumberVisitorVal pattern::visitor::Number::visitor_val;
pattern::visitor::NumberVisitorUnit pattern::visitor::Number::visitor_unit;

bool pattern::visitor::Unit::UnitOp(const msg::Number_Unit &unit1,
                                    const msg::Number_Unit &unit2,
                                    const msg::Number_Operator operator_,
                                    msg::Number_Unit *return_unit) {
  switch (operator_) {
    case msg::Number_Operator_PLUS:
    case msg::Number_Operator_MOINS: {
      BUGUSER(google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      *return_unit = unit1;
      return true;
    }
    case msg::Number_Operator_MULTIPLICATION: {
      return_unit->set_m(unit1.m() + unit2.m());
      return_unit->set_kg(unit1.kg() + unit2.kg());
      return_unit->set_s(unit1.s() + unit2.s());
      return_unit->set_a(unit1.a() + unit2.a());
      return_unit->set_k(unit1.k() + unit2.k());
      return_unit->set_mol(unit1.mol() + unit2.mol());
      return_unit->set_cd(unit1.cd() + unit2.cd());
      return true;
    }
    case msg::Number_Operator_DIVISION: {
      return_unit->set_m(unit1.m() - unit2.m());
      return_unit->set_kg(unit1.kg() - unit2.kg());
      return_unit->set_s(unit1.s() - unit2.s());
      return_unit->set_a(unit1.a() - unit2.a());
      return_unit->set_k(unit1.k() - unit2.k());
      return_unit->set_mol(unit1.mol() - unit2.mol());
      return_unit->set_cd(unit1.cd() - unit2.cd());
      return true;
    }
    case ::google::protobuf::kint32min:
    case ::google::protobuf::kint32max:
    default: { BUGPROG(false, false, "switch.\n"); }
  }
}

pattern::visitor::Number_Constant::Number_Constant(const uint32_t id,
                                                   const double value,
                                                   msg::Number_Unit *unit)
#ifdef ENABLE_VISITABLE_CACHE
    : cache_value_id_(0),
      cache_unit_id_(0)
#endif  // ENABLE_VISITABLE_CACHE
{
  msg::Number_Constant *constant = new msg::Number_Constant();
  constant->set_value(value);
  constant->set_allocated_unit(unit);

  message().set_id(id);
  message().set_allocated_constant(constant);
}

double pattern::visitor::Number_Constant::GetVal() const {
#ifdef ENABLE_VISITABLE_CACHE
  // Check cache.
  if (cache_value_id_ == message().id()) {
    return cache_value_;
  }
#endif  // ENABLE_VISITABLE_CACHE

  std::string return_value;
  BUGCONT(visitor_val.Visit(*this, &return_value), std::nan(""));

  msg::Double double_value;
  BUGLIB(double_value.ParseFromString(return_value), std::nan(""), "protobuf");

#ifdef ENABLE_VISITABLE_CACHE
  cache_value_ = double_value.value();
  cache_value_id_ = message().id();
#endif  // ENABLE_VISITABLE_CACHE

  return double_value.value();
}

msg::Number_Unit pattern::visitor::Number_Constant::GetUnit() const {
#ifdef ENABLE_VISITABLE_CACHE
  // Check cache.
  if (cache_unit_id_ == message().id()) {
    return cache_unit_;
  }
#endif  // ENABLE_VISITABLE_CACHE

  std::string return_unit;
  BUGCONT(visitor_unit.Visit(*this, &return_unit), msg::Number_Unit());

  msg::Number_Unit number_unit;
  BUGLIB(number_unit.ParseFromString(return_unit), msg::Number_Unit(),
         "protobuf");

#ifdef ENABLE_VISITABLE_CACHE
  cache_unit_ = number_unit;
  cache_unit_id_ = message().id();
#endif  // ENABLE_VISITABLE_CACHE

  return number_unit;
}

pattern::visitor::Number_NumOpNum::Number_NumOpNum(
    const uint32_t id, const Number &number1, msg::Number_Operator operator_,
    const Number &number2)
    : number1_(number1),
      number2_(number2)
#ifdef ENABLE_VISITABLE_CACHE
      ,
      cache_value1_id_(0),
      cache_value2_id_(0),
      cache_unit1_id_(0),
      cache_unit2_id_(0)
#endif  // ENABLE_VISITABLE_CACHE
{
  msg::Number_NumberOpNumber *number_operator_number =
      new msg::Number_NumberOpNumber();
  number_operator_number->set_id1(number1_.message().id());
  number_operator_number->set_operator_(operator_);
  number_operator_number->set_id2(number2_.message().id());

  message().set_id(id);
  message().set_allocated_number_op_number(number_operator_number);
}

double pattern::visitor::Number_NumOpNum::GetVal() const {
#ifdef ENABLE_VISITABLE_CACHE
  // Check cache.
  if ((cache_value1_id_ == number1_.message().id()) &&
      (cache_value2_id_ == number2_.message().id())) {
    return cache_value_;
  }
#endif  // ENABLE_VISITABLE_CACHE

  std::string return_value;
  BUGCONT(visitor_val.Visit(*this, &return_value), std::nan(""));

  msg::Double double_value;
  BUGLIB(double_value.ParseFromString(return_value), std::nan(""), "protobuf");

#ifdef ENABLE_VISITABLE_CACHE
  cache_value_ = double_value.value();
  cache_value1_id_ = number1_.message().id();
  cache_value2_id_ = number2_.message().id();
#endif  // ENABLE_VISITABLE_CACHE

  return double_value.value();
}

msg::Number_Unit pattern::visitor::Number_NumOpNum::GetUnit() const {
#ifdef ENABLE_VISITABLE_CACHE
  // Check cache.
  if ((cache_unit1_id_ == number1_.message().id()) &&
      (cache_unit2_id_ == number2_.message().id())) {
    return cache_unit_;
  }
#endif  // ENABLE_VISITABLE_CACHE

  std::string return_unit;
  BUGCONT(visitor_unit.Visit(*this, &return_unit), msg::Number_Unit());

  msg::Number_Unit number_unit;
  BUGLIB(number_unit.ParseFromString(return_unit), msg::Number_Unit(),
         "protobuf");

#ifdef ENABLE_VISITABLE_CACHE
  cache_unit_ = number_unit;
  cache_unit1_id_ = number1_.message().id();
  cache_unit2_id_ = number2_.message().id();
#endif  // ENABLE_VISITABLE_CACHE

  return number_unit;
}
