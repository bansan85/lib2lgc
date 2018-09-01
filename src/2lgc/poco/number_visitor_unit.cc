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
#include <2lgc/poco/math_number.pb.h>
#include <2lgc/poco/number.h>
#include <2lgc/poco/number_impl.h>
#include <2lgc/poco/number_visitor_unit.h>
#include <2lgc/poco/number_visitor_value.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/util/message_differencer.h>
#include <iostream>
#include <memory>

/** \class llgc::poco::NumberVisitorUnit
 * \brief The visitor that will return the unit.
 *
 * \fn llgc::poco::NumberVisitorUnit::NumberVisitorUnit()
 * \brief Default constructor.
 *
 *
 * \fn llgc::poco::NumberVisitorUnit::~NumberVisitorUnit()
 * \brief Default destructor.
 *
 *
 * \fn llgc::poco::NumberVisitorUnit::NumberVisitorUnit(NumberVisitorUnit&& other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::poco::NumberVisitorUnit::NumberVisitorUnit(NumberVisitorUnit const& other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn NumberVisitorUnit& llgc::poco::NumberVisitorUnit::operator=(NumberVisitorUnit&& other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn NumberVisitorUnit& llgc::poco::NumberVisitorUnit::operator=(NumberVisitorUnit const& other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

/** \brief Return the unit of a constant number.
 * \param data The constant number.
 * \param return_value This parameter will be changed only if Visit success.
 * \return true if success, false instead.
 */
bool llgc::poco::NumberVisitorUnit::Visit(const Number_Constant &data,
                                          std::string *return_value) const
{
  BUGPARAM(std::cout, 0, return_value != nullptr, false);

  BUGLIB(std::cout,
         data.Message().constant().unit().SerializeToString(return_value),
         false, "protobuf");

  return true;
}

/** \brief Return the unit of a number based on an operation of two numbers.
 * \param data The number.
 * \param return_value This parameter will be changed only if Visit success.
 * \return true if success, false instead.
 */
bool llgc::poco::NumberVisitorUnit::Visit(const Number_NumOpNum &data,
                                          std::string *return_value) const
{
  BUGPARAM(std::cout, 0, return_value != nullptr, false);

  llgc::protobuf::math::Number_Unit unit1;
  llgc::protobuf::math::Number_Unit unit2;
  llgc::protobuf::math::Number_Unit unit;
  std::string return_accept;
  BUGCONT(std::cout, data.Number1()->Accept(*this, &return_accept), false);
  BUGLIB(std::cout, unit1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(std::cout, data.Number2()->Accept(*this, &return_accept), false);
  BUGLIB(std::cout, unit2.ParseFromString(return_accept), false, "protobuf");

  switch (data.Message().number_op_number().operator_())
  {
    case llgc::protobuf::math::Number_Operator_PLUS:
    case llgc::protobuf::math::Number_Operator_MOINS:
    {
      BUGUSER(std::cout,
              google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      BUGLIB(std::cout, unit1.SerializeToString(return_value), false,
             "protobuf");
      return true;
    }
    case llgc::protobuf::math::Number_Operator_MULTIPLICATION:
    {
      BUGCONT(std::cout,
              Unit::UnitOp(unit1, unit2,
                           llgc::protobuf::math::Number_Operator_MULTIPLICATION,
                           &unit),
              false);
      BUGLIB(std::cout, unit.SerializeToString(return_value), false,
             "protobuf");
      return true;
    }
    case llgc::protobuf::math::Number_Operator_DIVISION:
    {
      NumberVisitorVal visitor_val;

      BUGCONT(std::cout, data.Number2()->Accept(visitor_val, &return_accept),
              false);
      llgc::protobuf::math::Double double_val;
      BUGLIB(std::cout, double_val.ParseFromString(return_accept), false,
             "protobuf");
      BUGUSER(std::cout,
              !llgc::math::Compare::AlmostEqualRelativeAndAbsD(
                  double_val.value(), 0., 1e-15, 1e-15),
              false, "Divide by zero.");
      BUGCONT(
          std::cout,
          Unit::UnitOp(unit1, unit2,
                       llgc::protobuf::math::Number_Operator_DIVISION, &unit),
          false);
      BUGLIB(std::cout, unit.SerializeToString(return_value), false,
             "protobuf");
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
