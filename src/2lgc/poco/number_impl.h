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

#ifndef POCO_NUMBER_IMPL_H_
#define POCO_NUMBER_IMPL_H_

#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/pattern/visitor/visitable.h>
#include <2lgc/poco/math_number.pb.h>
#include <2lgc/poco/number.h>
#include <cstdint>
#include <limits>
#include <memory>

namespace llgc::poco
{
class Number_Constant
    : virtual public Number,
      virtual public llgc::pattern::visitor::BaseVisitable<Number_Constant,
                                                           Number>
{
 public:
  Number_Constant(uint32_t id, double value,
                  llgc::protobuf::math::Number_Unit* unit);
#ifndef SWIG
  Number_Constant(Number_Constant&& other) = delete;
  Number_Constant(Number_Constant const& other) = delete;
  Number_Constant& operator=(Number_Constant&& other) = delete;
  Number_Constant& operator=(Number_Constant const& other) = delete;
#endif  // !SWIG
  ~Number_Constant() override = default;

  double GetVal() const override;
  llgc::protobuf::math::Number_Unit GetUnit() const override;

#ifndef DISABLE_VISITABLE_CACHE
 private:
  mutable uint32_t cache_value_id_ = std::numeric_limits<uint32_t>::max();
  mutable uint32_t cache_unit_id_ = std::numeric_limits<uint32_t>::max();
#endif  // DISABLE_VISITABLE_CACHE
};

class Number_NumOpNum
    : virtual public Number,
      virtual public llgc::pattern::visitor::BaseVisitable<Number_NumOpNum,
                                                           Number>
{
 public:
  Number_NumOpNum(uint32_t id, std::shared_ptr<const Number> number1,
                  llgc::protobuf::math::Number_Operator operator_,
                  std::shared_ptr<const Number> number2);
#ifndef SWIG
  Number_NumOpNum(Number_NumOpNum&& other) = delete;
  Number_NumOpNum(Number_NumOpNum const& other) = delete;
  Number_NumOpNum& operator=(Number_NumOpNum&& other) = delete;
  Number_NumOpNum& operator=(Number_NumOpNum const& other) = delete;
#endif  // !SWIG
  ~Number_NumOpNum() override = default;

  double GetVal() const override;
  llgc::protobuf::math::Number_Unit GetUnit() const override;

  const std::shared_ptr<const Number> Number1() const { return number1_; }
  const std::shared_ptr<const Number> Number2() const { return number2_; }

 private:
  const std::shared_ptr<const Number> number1_;
  const std::shared_ptr<const Number> number2_;

#ifndef DISABLE_VISITABLE_CACHE
  mutable uint32_t cache_value1_id_ = std::numeric_limits<uint32_t>::max();
  mutable uint32_t cache_value2_id_ = std::numeric_limits<uint32_t>::max();
  mutable uint32_t cache_unit1_id_ = std::numeric_limits<uint32_t>::max();
  mutable uint32_t cache_unit2_id_ = std::numeric_limits<uint32_t>::max();
#endif  // DISABLE_VISITABLE_CACHE
};

}  // namespace llgc::poco

#endif  // POCO_NUMBER_IMPL_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
