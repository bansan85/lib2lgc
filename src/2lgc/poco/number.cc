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
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/util/message_differencer.h>
#include <iostream>

/** \namespace llgc::poco
 * \brief Namespace that contains all classes to manipulate protobuf.
 *
 *
 * \class llgc::poco::Unit
 * \brief Class just to hold the UnitOp method.
 */

/** \brief Get the resulting unit when a operator is applied in two units.
 * \param unit1 First unit.
 * \param unit2 Second unit.
 * \param operator_ The operator.
 * \param return_unit The resulting unit. Must be already allocated.
 * \return true if the operator is successfully applied in the two units.
 */
bool llgc::poco::Unit::UnitOp(const llgc::protobuf::math::Number_Unit &unit1,
                              const llgc::protobuf::math::Number_Unit &unit2,
                              llgc::protobuf::math::Number_Operator operator_,
                              llgc::protobuf::math::Number_Unit *return_unit)
{
  switch (operator_)
  {
    case llgc::protobuf::math::Number_Operator_PLUS:
    case llgc::protobuf::math::Number_Operator_MOINS:
    {
      BUGUSER(std::cout,
              google::protobuf::util::MessageDifferencer::Equals(unit1, unit2),
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
      BUGPROG(std::cout, false, false, "switch.\n");
    }
  }
}

/** \class llgc::poco::Number
 * \brief Abstract class that represent a read-only number and it's unit.
 *
 *
 * \fn llgc::poco::Number::Number()
 * \brief Default constructor.
 *
 *
 * \fn llgc::poco::Number::~Number()
 * \brief Default destructor.
 *
 *
 * \fn llgc::poco::Number::Number(Number &&other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::poco::Number::Number(Number const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn Number & llgc::poco::Number::operator=(Number &&other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn Number & llgc::poco::Number::operator=(Number const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn double llgc::poco::Number::GetVal() const
 * \brief Get the value of the number.
 * \return The floating number. If failed, a nan number.
 *
 *
 * \fn llgc::protobuf::math::Number_Unit llgc::poco::Number::GetUnit() const
 * \brief Get the unit of the number.
 * \return The unit. If failed, the default Number_Unit.
 *
 *
 * \var llgc::poco::Number::cache_value_
 * \brief If cache enabled, the value.
 *
 *
 * \var llgc::poco::Number::cache_unit_
 * \brief If cache enabled, the unit.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
