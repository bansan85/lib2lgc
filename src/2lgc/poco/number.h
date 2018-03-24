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

#ifndef POCO_NUMBER_H_
#define POCO_NUMBER_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/visitor/visitable.h>
#include <2lgc/poco/number.pb.h>

namespace pattern
{
namespace visitor
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
  static bool UnitOp(const msg::Number_Unit &unit1,
                     const msg::Number_Unit &unit2,
                     const msg::Number_Operator operator_,
                     msg::Number_Unit *return_unit) CHK;
};

/**
 * @brief Abstract class that represent a read-only number and it's unit.
 */
class Number : public InterfaceVisitable<msg::Number>
{
 public:
  /**
   * @brief Default constructor.
   */
  Number()
#ifdef ENABLE_VISITABLE_CACHE
      : cache_value_(0.), cache_unit_()
#endif  // ENABLE_VISITABLE_CACHE
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
  virtual msg::Number_Unit GetUnit() const = 0;

#ifdef ENABLE_VISITABLE_CACHE
 protected:
  /**
   * @brief If cache enabled, the value.
   */
  mutable double cache_value_;
  /**
   * @brief If cache enabled, the unit.
   */
  mutable msg::Number_Unit cache_unit_;
#endif  // ENABLE_VISITABLE_CACHE
};

}  // namespace visitor
}  // namespace pattern

#endif  // POCO_NUMBER_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
