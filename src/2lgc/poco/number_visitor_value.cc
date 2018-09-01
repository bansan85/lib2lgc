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

/** \class llgc::poco::NumberVisitorVal
 * \brief The visitor that will return the floating value.
 *
 * \fn llgc::poco::NumberVisitorVal::NumberVisitorVal()
 * \brief Default constructor.
 *
 *
 * \fn llgc::poco::NumberVisitorVal::~NumberVisitorVal()
 * \brief Default destructor.
 *
 *
 * \fn llgc::poco::NumberVisitorVal::NumberVisitorVal(NumberVisitorVal&& other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::poco::NumberVisitorVal::NumberVisitorVal(NumberVisitorVal const& other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn NumberVisitorVal& llgc::poco::NumberVisitorVal::operator=(NumberVisitorVal&& other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn NumberVisitorVal& llgc::poco::NumberVisitorVal::operator=(NumberVisitorVal const& other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

/** \brief Return the floating value of a constant number.
 * \param data The constant number.
 * \param return_value This parameter will be changed only if Visit success.
 * \return true if success, false instead.
 */
bool llgc::poco::NumberVisitorVal::Visit(const Number_Constant &data,
                                         std::string *return_value) const
{
  BUGPARAM(std::cout, 0, return_value != nullptr, false);

  llgc::protobuf::math::Double val;
  val.set_value(data.Message().constant().value());

  BUGLIB(std::cout, val.SerializeToString(return_value), false, "protobuf");
  return true;
}

/** \brief Return the floating value of a number based on an operation of two
 *         numbers.
 * \param data The number.
 * \param return_value This parameter will be changed only if Visit success.
 * \return true if success, false instead.
 */
bool llgc::poco::NumberVisitorVal::Visit(const Number_NumOpNum &data,
                                         std::string *return_value) const
{
  BUGPARAM(std::cout, 0, return_value != nullptr, false);

  llgc::protobuf::math::Double val1;
  llgc::protobuf::math::Double val2;
  llgc::protobuf::math::Double val;
  std::string return_accept;
  BUGCONT(std::cout, data.Number1()->Accept(*this, &return_accept), false);
  BUGLIB(std::cout, val1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(std::cout, data.Number2()->Accept(*this, &return_accept), false);
  BUGLIB(std::cout, val2.ParseFromString(return_accept), false, "protobuf");

  NumberVisitorUnit visitor_unit;
  llgc::protobuf::math::Number_Unit unit1;
  llgc::protobuf::math::Number_Unit unit2;
  BUGCONT(std::cout, data.Number1()->Accept(visitor_unit, &return_accept),
          false);
  BUGLIB(std::cout, unit1.ParseFromString(return_accept), false, "protobuf");
  BUGCONT(std::cout, data.Number2()->Accept(visitor_unit, &return_accept),
          false);
  BUGLIB(std::cout, unit2.ParseFromString(return_accept), false, "protobuf");

  switch (data.Message().number_op_number().operator_())
  {
    case llgc::protobuf::math::Number_Operator_PLUS:
    {
      BUGUSER(std::cout,
              google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      val.set_value(val1.value() + val2.value());
      break;
    }
    case llgc::protobuf::math::Number_Operator_MOINS:
    {
      BUGUSER(std::cout,
              google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
              false, "Incompatible unit.\n");
      val.set_value(val1.value() - val2.value());
      break;
    }
    case llgc::protobuf::math::Number_Operator_MULTIPLICATION:
    {
      val.set_value(val1.value() * val2.value());
      break;
    }
    case llgc::protobuf::math::Number_Operator_DIVISION:
    {
      BUGUSER(std::cout,
              !llgc::math::Compare::AlmostEqualRelativeAndAbsD(val2.value(), 0.,
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

  BUGLIB(std::cout, val.SerializeToString(return_value), false, "protobuf");
  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
