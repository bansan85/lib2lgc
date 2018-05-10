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

#ifndef POCO_NUMBER_H_
#define POCO_NUMBER_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/visitor/visitable.h>
#include <2lgc/poco/math_number.pb.h>

/**
 * @brief Namespace that contains all classes to manipulate protobuf.
 */
namespace llgc::poco
{
/**
 * @brief Class just to hold the UnitOp method.
 */
class Unit
{
 public:
  /**
   * @brief Get the resulting unit when a operator is applied in two units.
   *
   * @param unit1 First unit.
   * @param unit2 Second unit.
   * @param operator_ The operator.
   * @param return_unit The resulting unit. Must be already allocated.
   *
   * @return true if the operator is successfully applied in the two units.
   */
  static bool UnitOp(const llgc::protobuf::math::Number_Unit &unit1,
                     const llgc::protobuf::math::Number_Unit &unit2,
                     const llgc::protobuf::math::Number_Operator operator_,
                     llgc::protobuf::math::Number_Unit *return_unit) CHK;
};

/**
 * @brief Abstract class that represent a read-only number and it's unit.
 */
class Number : public llgc::pattern::visitor::InterfaceVisitable<
                   llgc::protobuf::math::Number>
{
 public:
  /**
   * @brief Default constructor.
   */
  Number()
#ifndef DISABLE_VISITABLE_CACHE
      : cache_value_(0.), cache_unit_()
#endif  // DISABLE_VISITABLE_CACHE
  {
  }
  /**
   * @brief Default destructor.
   */
  virtual ~Number() {}

  /**
   * @brief Get the value of the number.
   *
   * @return The floating number. If failed, a nan number.
   */
  virtual double GetVal() const = 0;
  /**
   * @brief Get the unit of the number.
   *
   * @return The unit. If failed, the default Number_Unit.
   */
  virtual llgc::protobuf::math::Number_Unit GetUnit() const = 0;

#ifndef DISABLE_VISITABLE_CACHE
 protected:
  /**
   * @brief If cache enabled, the value.
   */
  mutable double cache_value_;
  /**
   * @brief If cache enabled, the unit.
   */
  mutable llgc::protobuf::math::Number_Unit cache_unit_;
#endif  // DISABLE_VISITABLE_CACHE
};

}  // namespace llgc::poco

#endif  // POCO_NUMBER_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
