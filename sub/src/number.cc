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

#include "number.h"

// Google protobuf library
#include <google/protobuf/util/message_differencer.h>

// lib2lgcPoco
#include <raw.pb.h>

// macros
#include <handle_error.h>

// C++ system
#include <cmath>

// Static variables
pattern::visitor::NumberVisitorVal pattern::visitor::Number::visitor_val;
pattern::visitor::NumberVisitorUnit pattern::visitor::Number::visitor_unit;

bool pattern::visitor::Unit::UnitOp(const msg::Number_Unit &unit1,
                                    const msg::Number_Unit &unit2,
                                    const msg::Number_Operator operator_,
                                    msg::Number_Unit *return_unit)
{
  switch (operator_)
  {
    case msg::Number_Operator_PLUS:
    case msg::Number_Operator_MOINS:
    {
      BUGUSER(google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      *return_unit = unit1;
      return true;
    }
    case msg::Number_Operator_MULTIPLICATION:
    {
      return_unit->set_m(unit1.m() + unit2.m());
      return_unit->set_kg(unit1.kg() + unit2.kg());
      return_unit->set_s(unit1.s() + unit2.s());
      return_unit->set_a(unit1.a() + unit2.a());
      return_unit->set_k(unit1.k() + unit2.k());
      return_unit->set_mol(unit1.mol() + unit2.mol());
      return_unit->set_cd(unit1.cd() + unit2.cd());
      return true;
    }
    case msg::Number_Operator_DIVISION:
    {
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
    default:
    {
      BUGPROG(false, false, "switch.\n");
    }
  }
}

pattern::visitor::Number_Constant::Number_Constant(const uint32_t id,
                                                   const double value,
                                                   msg::Number_Unit *unit)
#ifdef ENABLE_VISITABLE_CACHE
    : cache_value_id_(std::numeric_limits<uint32_t>::max()),
      cache_unit_id_(std::numeric_limits<uint32_t>::max())
#endif  // ENABLE_VISITABLE_CACHE
{
  msg::Number_Constant *constant = new msg::Number_Constant();
  constant->set_value(value);
  constant->set_allocated_unit(unit);

  message().set_id(id);
  message().set_allocated_constant(constant);
}

double pattern::visitor::Number_Constant::GetVal() const
{
#ifdef ENABLE_VISITABLE_CACHE
  // Check cache.
  if (cache_value_id_ == message().id())
  {
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

msg::Number_Unit pattern::visitor::Number_Constant::GetUnit() const
{
#ifdef ENABLE_VISITABLE_CACHE
  // Check cache.
  if (cache_unit_id_ == message().id())
  {
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
    const uint32_t id, const std::shared_ptr<const Number> &number1,
    msg::Number_Operator operator_,
    const std::shared_ptr<const Number> &number2)
    : number1_(number1),
      number2_(number2)
#ifdef ENABLE_VISITABLE_CACHE
      ,
      cache_value1_id_(std::numeric_limits<uint32_t>::max()),
      cache_value2_id_(std::numeric_limits<uint32_t>::max()),
      cache_unit1_id_(std::numeric_limits<uint32_t>::max()),
      cache_unit2_id_(std::numeric_limits<uint32_t>::max())
#endif  // ENABLE_VISITABLE_CACHE
{
  msg::Number_NumberOpNumber *number_operator_number =
      new msg::Number_NumberOpNumber();
  number_operator_number->set_id1(number1_->message().id());
  number_operator_number->set_operator_(operator_);
  number_operator_number->set_id2(number2_->message().id());

  message().set_id(id);
  message().set_allocated_number_op_number(number_operator_number);
}

double pattern::visitor::Number_NumOpNum::GetVal() const
{
#ifdef ENABLE_VISITABLE_CACHE
  // Check cache.
  if ((cache_value1_id_ == number1_->message().id()) &&
      (cache_value2_id_ == number2_->message().id()))
  {
    return cache_value_;
  }
#endif  // ENABLE_VISITABLE_CACHE

  std::string return_value;
  BUGCONT(visitor_val.Visit(*this, &return_value), std::nan(""));

  msg::Double double_value;
  BUGLIB(double_value.ParseFromString(return_value), std::nan(""), "protobuf");

#ifdef ENABLE_VISITABLE_CACHE
  cache_value_ = double_value.value();
  cache_value1_id_ = number1_->message().id();
  cache_value2_id_ = number2_->message().id();
#endif  // ENABLE_VISITABLE_CACHE

  return double_value.value();
}

msg::Number_Unit pattern::visitor::Number_NumOpNum::GetUnit() const
{
#ifdef ENABLE_VISITABLE_CACHE
  // Check cache.
  if ((cache_unit1_id_ == number1_->message().id()) &&
      (cache_unit2_id_ == number2_->message().id()))
  {
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
  cache_unit1_id_ = number1_->message().id();
  cache_unit2_id_ = number2_->message().id();
#endif  // ENABLE_VISITABLE_CACHE

  return number_unit;
}
