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

#ifndef POCO_NUMBER_IMPL_H_
#define POCO_NUMBER_IMPL_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/visitor/visitable.h>
#include <2lgc/poco/number.h>
#include <2lgc/poco/number.pb.h>
#include <cstdint>
#include <memory>

/**
 * @brief Namespace that contains all classes to manipulate protobuf.
 */
namespace llgc::poco
{
/**
 * @brief A const number.
 */
class Number_Constant
    : virtual public Number,
      virtual public llgc::pattern::visitor::BaseVisitable<Number_Constant,
                                                           Number>
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
  ~Number_Constant() override {}

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

#ifndef DISABLE_VISITABLE_CACHE
 private:
  /**
   * @brief The id of the cached value.
   */
  mutable uint32_t cache_value_id_;
  /**
   * @brief The id of the cached unit.
   */
  mutable uint32_t cache_unit_id_;
#endif  // DISABLE_VISITABLE_CACHE
};

/**
 * @brief A const number based on an operation of two numbers.
 */
class Number_NumOpNum
    : virtual public Number,
      virtual public llgc::pattern::visitor::BaseVisitable<Number_NumOpNum,
                                                           Number>
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
  Number_NumOpNum(const uint32_t id, std::shared_ptr<const Number> number1,
                  msg::Number_Operator operator_,
                  std::shared_ptr<const Number> number2);
  /**
   * @brief Default destructor.
   */
  ~Number_NumOpNum() override {}

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
  const std::shared_ptr<const Number> Number1() const { return number1_; }
  /**
   * @brief Return the number 2.
   *
   * @return The number 2.
   */
  const std::shared_ptr<const Number> Number2() const { return number2_; }

 private:
  /**
   * @brief The number 1.
   */
  const std::shared_ptr<const Number> number1_;
  /**
   * @brief The number 2.
   */
  const std::shared_ptr<const Number> number2_;

#ifndef DISABLE_VISITABLE_CACHE
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
#endif  // DISABLE_VISITABLE_CACHE
};

}  // namespace llgc::poco

#endif  // POCO_NUMBER_IMPL_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
