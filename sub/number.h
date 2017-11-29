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

#ifndef NUMBER_H_
#define NUMBER_H_

#include "config.h"

// C++ system
#include <string>

// lib2lgcPoco
#include <number.pb.h>

// lib2lgcVisitor
#include <visitable.h>
#include <visitor.h>

// Current project
#include "number_visitor_unit.h"
#include "number_visitor_value.h"

namespace pattern {
namespace visitor {

/**
 * @brief Class just to hold the UnitOp method.
 */
class Unit {
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
  static bool UnitOp(const msg::Number_Unit &unit1,
                     const msg::Number_Unit &unit2,
                     const msg::Number_Operator operator_,
                     msg::Number_Unit *return_unit) CHK;
};

class Number : public InterfaceVisitable<msg::Number> {
 public:
  Number()
#ifdef ENABLE_VISITABLE_CACHE
      : cache_value_(0.),
        cache_unit_()
#endif  // ENABLE_VISITABLE_CACHE
  {
  }
  virtual ~Number() {}

  virtual double GetVal() const = 0;
  virtual msg::Number_Unit GetUnit() const = 0;

 protected:
  static pattern::visitor::NumberVisitorVal visitor_val;
  static pattern::visitor::NumberVisitorUnit visitor_unit;

#ifdef ENABLE_VISITABLE_CACHE
  mutable double cache_value_;
  mutable msg::Number_Unit cache_unit_;
#endif  // ENABLE_VISITABLE_CACHE
};

class Number_Constant : virtual public Number,
                        virtual public BaseVisitable<Number_Constant, Number> {
 public:
  Number_Constant(const uint32_t id, const double value,
                  msg::Number_Unit *unit);
  virtual ~Number_Constant() {}

  double GetVal() const override;
  msg::Number_Unit GetUnit() const override;

 private:
#ifdef ENABLE_VISITABLE_CACHE
  mutable unsigned int cache_value_id_;
  mutable unsigned int cache_unit_id_;
#endif  // ENABLE_VISITABLE_CACHE
};

class Number_NumOpNum : virtual public Number,
                        virtual public BaseVisitable<Number_NumOpNum, Number> {
 public:
  Number_NumOpNum(const uint32_t id, const Number &number1,
                  msg::Number_Operator operator_, const Number &number2);
  virtual ~Number_NumOpNum() {}

  double GetVal() const override;
  msg::Number_Unit GetUnit() const override;

  const Number &number1() const { return number1_; }
  const Number &number2() const { return number2_; }

 private:
  const Number &number1_;
  const Number &number2_;

#ifdef ENABLE_VISITABLE_CACHE
  mutable unsigned int cache_value1_id_;
  mutable unsigned int cache_value2_id_;
  mutable unsigned int cache_unit1_id_;
  mutable unsigned int cache_unit2_id_;
#endif  // ENABLE_VISITABLE_CACHE
};

}  // namespace visitor
}  // namespace pattern

#endif  // NUMBER_H_
