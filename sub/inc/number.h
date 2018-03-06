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

#ifndef NUMBER_H_
#define NUMBER_H_

#include <bits/stdint-uintn.h>
#include <compat.h>
#include <number.pb.h>
#include <visitable.h>
#include <memory>

namespace pattern
{
namespace visitor
{
class NumberVisitorUnit;
class NumberVisitorVal;
}
}

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

 protected:
  /**
   * @brief The visitor to get the value.
   */
  static pattern::visitor::NumberVisitorVal visitor_val;
  /**
   * @brief The visitor to get the unit.
   */
  static pattern::visitor::NumberVisitorUnit visitor_unit;

#ifdef ENABLE_VISITABLE_CACHE
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

/**
 * @brief A const number.
 */
class Number_Constant : virtual public Number,
                        virtual public BaseVisitable<Number_Constant, Number>
{
 public:
  /**
   * @brief Constructor to initialize the value and the unit.
   *
   * @param id The id of the number.
   * @param value Float value.
   * @param unit The unit of the number. The class must be allocated on the
   * stack or in a arena.
   */
  Number_Constant(const uint32_t id, const double value,
                  msg::Number_Unit *unit);
  /**
   * @brief Default destructor. No need to delete unit. It will be free
   * automatically.
   */
  virtual ~Number_Constant() {}

  /**
   * @brief Get the value of the number.
   *
   * @return The floating number.
   */
  double GetVal() const override;
  /**
   * @brief Get the unit of the number.
   *
   * @return The unit.
   */
  msg::Number_Unit GetUnit() const override;

 private:
#ifdef ENABLE_VISITABLE_CACHE
  /**
   * @brief The id of the cached value.
   */
  mutable uint32_t cache_value_id_;
  /**
   * @brief The id of the cached unit.
   */
  mutable uint32_t cache_unit_id_;
#endif  // ENABLE_VISITABLE_CACHE
};

/**
 * @brief A const number based on an operation of two numbers.
 */
class Number_NumOpNum : virtual public Number,
                        virtual public BaseVisitable<Number_NumOpNum, Number>
{
 public:
  /**
   * @brief Constructor to initialize the value and the unit.
   *
   * @param id The id of the number.
   * @param number1 The number one.
   * @param operator_ The operator.
   * @param number2 The number two.
   */
  Number_NumOpNum(const uint32_t id,
                  const std::shared_ptr<const Number> &number1,
                  msg::Number_Operator operator_,
                  const std::shared_ptr<const Number> &number2);
  /**
   * @brief Default destructor.
   */
  virtual ~Number_NumOpNum() {}

  /**
   * @brief Get the value of the number.
   *
   * @return The floating number.
   */
  double GetVal() const override;
  /**
   * @brief Get the unit of the number.
   *
   * @return Return a calculated unit base on the operator. If failed, the unit
   * will be default.
   */
  msg::Number_Unit GetUnit() const override;

  /**
   * @brief Return the number 1.
   *
   * @return The number 1.
   */
  const std::shared_ptr<const Number> number1() const { return number1_; }
  /**
   * @brief Return the number 2.
   *
   * @return The number 2.
   */
  const std::shared_ptr<const Number> number2() const { return number2_; }

 private:
  /**
   * @brief The number 1.
   */
  const std::shared_ptr<const Number> number1_;
  /**
   * @brief The number 2.
   */
  const std::shared_ptr<const Number> number2_;

#ifdef ENABLE_VISITABLE_CACHE
  /**
   * @brief The id of the number 1 for the cached value.
   */
  mutable uint32_t cache_value1_id_;
  /**
   * @brief The id of the number 2 for the cached value.
   */
  mutable uint32_t cache_value2_id_;
  /**
   * @brief The id of the unit 1 for the cached unit.
   */
  mutable uint32_t cache_unit1_id_;
  /**
   * @brief The id of the unit 2 for the cached unit.
   */
  mutable uint32_t cache_unit2_id_;
#endif  // ENABLE_VISITABLE_CACHE
};

}  // namespace visitor
}  // namespace pattern

#endif  // NUMBER_H_
