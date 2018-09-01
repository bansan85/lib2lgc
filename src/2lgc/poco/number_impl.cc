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
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

/** \class llgc::poco::Number_Constant
 * \brief A const number.
 */

/** \brief Constructor to initialize the value and the unit.
 * \param id The id of the number.
 * \param value Float value.
 * \param unit The unit of the number. The class must be allocated on the stack
 *        or in a arena.
 */
llgc::poco::Number_Constant::Number_Constant(
    uint32_t id, double value, llgc::protobuf::math::Number_Unit *unit)
{
  auto constant = std::make_unique<llgc::protobuf::math::Number_Constant>();
  constant->set_value(value);
  constant->set_allocated_unit(unit);

  Message().set_id(id);
  Message().set_allocated_constant(constant.release());
}

/** \fn llgc::poco::Number_Constant::~Number_Constant()
 * \brief Default destructor. unit will be free automatically.
 *
 * 
 * \fn llgc::poco::Number_Constant::Number_Constant(Number_Constant&& other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::poco::Number_Constant::Number_Constant(Number_Constant const& other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn Number_Constant& llgc::poco::Number_Constant::operator=(Number_Constant&& other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn Number_Constant& llgc::poco::Number_Constant::operator=(Number_Constant const& other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

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
  BUGCONT(std::cout, visitor_val.Visit(*this, &return_value), std::nan(""));

  llgc::protobuf::math::Double double_value;
  BUGLIB(std::cout, double_value.ParseFromString(return_value), std::nan(""),
         "protobuf");

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
  BUGCONT(std::cout, visitor_unit.Visit(*this, &return_unit),
          llgc::protobuf::math::Number_Unit());

  llgc::protobuf::math::Number_Unit number_unit;
  BUGLIB(std::cout, number_unit.ParseFromString(return_unit),
         llgc::protobuf::math::Number_Unit(), "protobuf");

#ifndef DISABLE_VISITABLE_CACHE
  cache_unit_ = number_unit;
  cache_unit_id_ = Message().id();
#endif  // DISABLE_VISITABLE_CACHE

  return number_unit;
}

/** \var llgc::poco::Number_Constant::cache_value_id_
 * \brief The id of the cached value.
 *
 *
 * \var llgc::poco::Number_Constant::cache_unit_id_
 * \brief The id of the cached unit.
 */

/** \class llgc::poco::Number_NumOpNum
 * \brief A const number based on an operation of two numbers.
 */

/** \brief Constructor to initialize the value and the unit.
 * \param id The id of the number.
 * \param number1 The number one.
 * \param operator_ The operator.
 * \param number2 The number two.
 */
llgc::poco::Number_NumOpNum::Number_NumOpNum(
    uint32_t id, std::shared_ptr<const Number> number1,
    llgc::protobuf::math::Number_Operator operator_,
    std::shared_ptr<const Number> number2)
    : number1_(std::move(number1)), number2_(std::move(number2))
{
  auto number_operator_number =
      std::make_unique<llgc::protobuf::math::Number_NumberOpNumber>();
  number_operator_number->set_id1(number1_->Message().id());
  number_operator_number->set_operator_(operator_);
  number_operator_number->set_id2(number2_->Message().id());

  Message().set_id(id);
  Message().set_allocated_number_op_number(number_operator_number.release());
}

/** \fn llgc::poco::Number_NumOpNum::~Number_NumOpNum()
 * \brief Default destructor. unit will be free automatically.
 *
 * 
 * \fn llgc::poco::Number_NumOpNum::Number_NumOpNum(Number_NumOpNum&& other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::poco::Number_NumOpNum::Number_NumOpNum(Number_NumOpNum const& other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn Number_NumOpNum& llgc::poco::Number_NumOpNum::operator=(Number_NumOpNum&& other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn Number_NumOpNum& llgc::poco::Number_NumOpNum::operator=(Number_NumOpNum const& other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

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
  BUGCONT(std::cout, visitor_val.Visit(*this, &return_value), std::nan(""));

  llgc::protobuf::math::Double double_value;
  BUGLIB(std::cout, double_value.ParseFromString(return_value), std::nan(""),
         "protobuf");

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
  BUGCONT(std::cout, visitor_unit.Visit(*this, &return_unit),
          llgc::protobuf::math::Number_Unit());

  llgc::protobuf::math::Number_Unit number_unit;
  BUGLIB(std::cout, number_unit.ParseFromString(return_unit),
         llgc::protobuf::math::Number_Unit(), "protobuf");

#ifndef DISABLE_VISITABLE_CACHE
  cache_unit_ = number_unit;
  cache_unit1_id_ = number1_->Message().id();
  cache_unit2_id_ = number2_->Message().id();
#endif  // DISABLE_VISITABLE_CACHE

  return number_unit;
}

/** \fn const std::shared_ptr<const Number> llgc::poco::Number_NumOpNum::Number1() const
 * \brief Return the number 1.
 * \return The number 1.
 *
 *
 * \fn const std::shared_ptr<const Number> llgc::poco::Number_NumOpNum::Number2() const
 * \brief Return the number 2.
 * \return The number 2.
 *
 *
 * \var llgc::poco::Number_NumOpNum::number1_
 * \brief The number 1.
 *
 *
 * \var llgc::poco::Number_NumOpNum::number2_
 * \brief The number 2.
 *
 *
 * \var llgc::poco::Number_NumOpNum::cache_value1_id_
 * \brief The id of the number 1 for the cached value.
 *
 *
 * \var llgc::poco::Number_NumOpNum::cache_value2_id_
 * \brief The id of the number 2 for the cached value.
 *
 *
 * \var llgc::poco::Number_NumOpNum::cache_unit1_id_
 * \brief The id of the unit 1 for the cached unit.
 *
 *
 * \var llgc::poco::Number_NumOpNum::cache_unit2_id_
 * \brief The id of the unit 2 for the cached unit.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
