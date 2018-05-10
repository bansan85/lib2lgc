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
#include <2lgc/poco/math_number.pb.h>
#include <2lgc/poco/number.h>
#include <2lgc/poco/number_impl.h>
#include <2lgc/poco/number_visitor_unit.h>
#include <2lgc/poco/number_visitor_value.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/util/message_differencer.h>
#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <utility>

bool llgc::poco::Unit::UnitOp(
    const llgc::protobuf::math::Number_Unit &unit1,
    const llgc::protobuf::math::Number_Unit &unit2,
    const llgc::protobuf::math::Number_Operator operator_,
    llgc::protobuf::math::Number_Unit *return_unit)
{
  switch (operator_)
  {
    case llgc::protobuf::math::Number_Operator_PLUS:
    case llgc::protobuf::math::Number_Operator_MOINS:
    {
      BUGUSER(google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      *return_unit = unit1;
      return true;
    }
    case llgc::protobuf::math::Number_Operator_MULTIPLICATION:
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
    case llgc::protobuf::math::Number_Operator_DIVISION:
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

llgc::poco::Number_Constant::Number_Constant(
    const uint32_t id, const double value,
    llgc::protobuf::math::Number_Unit *unit)
#ifndef DISABLE_VISITABLE_CACHE
    : cache_value_id_(std::numeric_limits<uint32_t>::max()),
      cache_unit_id_(std::numeric_limits<uint32_t>::max())
#endif  // DISABLE_VISITABLE_CACHE
{
  auto constant = std::make_unique<llgc::protobuf::math::Number_Constant>();
  constant->set_value(value);
  constant->set_allocated_unit(unit);

  Message().set_id(id);
  Message().set_allocated_constant(constant.release());
}

double llgc::poco::Number_Constant::GetVal() const
{
#ifndef DISABLE_VISITABLE_CACHE
  // Check cache.
  if (cache_value_id_ == Message().id())
  {
    return cache_value_;
  }
#endif  // DISABLE_VISITABLE_CACHE

  std::string return_value;
  llgc::poco::NumberVisitorVal visitor_val;
  BUGCONT(visitor_val.Visit(*this, &return_value), std::nan(""));

  llgc::protobuf::math::Double double_value;
  BUGLIB(double_value.ParseFromString(return_value), std::nan(""), "protobuf");

#ifndef DISABLE_VISITABLE_CACHE
  cache_value_ = double_value.value();
  cache_value_id_ = Message().id();
#endif  // DISABLE_VISITABLE_CACHE

  return double_value.value();
}

llgc::protobuf::math::Number_Unit llgc::poco::Number_Constant::GetUnit() const
{
#ifndef DISABLE_VISITABLE_CACHE
  // Check cache.
  if (cache_unit_id_ == Message().id())
  {
    return cache_unit_;
  }
#endif  // DISABLE_VISITABLE_CACHE

  std::string return_unit;
  llgc::poco::NumberVisitorUnit visitor_unit;
  BUGCONT(visitor_unit.Visit(*this, &return_unit),
          llgc::protobuf::math::Number_Unit());

  llgc::protobuf::math::Number_Unit number_unit;
  BUGLIB(number_unit.ParseFromString(return_unit),
         llgc::protobuf::math::Number_Unit(), "protobuf");

#ifndef DISABLE_VISITABLE_CACHE
  cache_unit_ = number_unit;
  cache_unit_id_ = Message().id();
#endif  // DISABLE_VISITABLE_CACHE

  return number_unit;
}

llgc::poco::Number_NumOpNum::Number_NumOpNum(
    const uint32_t id, std::shared_ptr<const Number> number1,
    llgc::protobuf::math::Number_Operator operator_,
    std::shared_ptr<const Number> number2)
    : number1_(std::move(number1)),
      number2_(std::move(number2))
#ifndef DISABLE_VISITABLE_CACHE
      ,
      cache_value1_id_(std::numeric_limits<uint32_t>::max()),
      cache_value2_id_(std::numeric_limits<uint32_t>::max()),
      cache_unit1_id_(std::numeric_limits<uint32_t>::max()),
      cache_unit2_id_(std::numeric_limits<uint32_t>::max())
#endif  // DISABLE_VISITABLE_CACHE
{
  auto number_operator_number =
      std::make_unique<llgc::protobuf::math::Number_NumberOpNumber>();
  number_operator_number->set_id1(number1_->Message().id());
  number_operator_number->set_operator_(operator_);
  number_operator_number->set_id2(number2_->Message().id());

  Message().set_id(id);
  Message().set_allocated_number_op_number(number_operator_number.release());
}

double llgc::poco::Number_NumOpNum::GetVal() const
{
#ifndef DISABLE_VISITABLE_CACHE
  // Check cache.
  if ((cache_value1_id_ == number1_->Message().id()) &&
      (cache_value2_id_ == number2_->Message().id()))
  {
    return cache_value_;
  }
#endif  // DISABLE_VISITABLE_CACHE

  std::string return_value;
  llgc::poco::NumberVisitorVal visitor_val;
  BUGCONT(visitor_val.Visit(*this, &return_value), std::nan(""));

  llgc::protobuf::math::Double double_value;
  BUGLIB(double_value.ParseFromString(return_value), std::nan(""), "protobuf");

#ifndef DISABLE_VISITABLE_CACHE
  cache_value_ = double_value.value();
  cache_value1_id_ = number1_->Message().id();
  cache_value2_id_ = number2_->Message().id();
#endif  // DISABLE_VISITABLE_CACHE

  return double_value.value();
}

llgc::protobuf::math::Number_Unit llgc::poco::Number_NumOpNum::GetUnit() const
{
#ifndef DISABLE_VISITABLE_CACHE
  // Check cache.
  if ((cache_unit1_id_ == number1_->Message().id()) &&
      (cache_unit2_id_ == number2_->Message().id()))
  {
    return cache_unit_;
  }
#endif  // DISABLE_VISITABLE_CACHE

  std::string return_unit;
  llgc::poco::NumberVisitorUnit visitor_unit;
  BUGCONT(visitor_unit.Visit(*this, &return_unit),
          llgc::protobuf::math::Number_Unit());

  llgc::protobuf::math::Number_Unit number_unit;
  BUGLIB(number_unit.ParseFromString(return_unit),
         llgc::protobuf::math::Number_Unit(), "protobuf");

#ifndef DISABLE_VISITABLE_CACHE
  cache_unit_ = number_unit;
  cache_unit1_id_ = number1_->Message().id();
  cache_unit2_id_ = number2_->Message().id();
#endif  // DISABLE_VISITABLE_CACHE

  return number_unit;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
