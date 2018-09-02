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

#include <2lgc/compat.h>
#include <2lgc/pattern/visitor/visitable.h>
#include <2lgc/poco/math_number.pb.h>

namespace llgc::poco
{
class Unit
{
 public:
  static bool UnitOp(const llgc::protobuf::math::Number_Unit &unit1,
                     const llgc::protobuf::math::Number_Unit &unit2,
                     llgc::protobuf::math::Number_Operator operator_,
                     llgc::protobuf::math::Number_Unit *return_unit) CHK;
};

/**
 * @brief Abstract class that represent a read-only number and it's unit.
 */
class Number : public llgc::pattern::visitor::InterfaceVisitable<
                   llgc::protobuf::math::Number>
{
 public:
  Number() = default;
  ~Number() override = default;

#ifndef SWIG
  Number(Number &&other) = delete;
  Number(Number const &other) = delete;
  Number &operator=(Number &&other) = delete;
  Number &operator=(Number const &other) = delete;
#endif  // !SWIG

  virtual double GetVal() const = 0;
  virtual llgc::protobuf::math::Number_Unit GetUnit() const = 0;

#ifndef DISABLE_VISITABLE_CACHE
 protected:
  mutable double cache_value_ = 0.;
  mutable llgc::protobuf::math::Number_Unit cache_unit_;
#endif  // DISABLE_VISITABLE_CACHE
};

}  // namespace llgc::poco

#endif  // POCO_NUMBER_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
