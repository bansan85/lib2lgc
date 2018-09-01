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

#ifndef POCO_NUMBER_VISITOR_VALUE_H_
#define POCO_NUMBER_VISITOR_VALUE_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/visitor/visitor.h>
#include <string>

namespace llgc::poco
{
class Number_Constant;
class Number_NumOpNum;

class NumberVisitorVal
    : public llgc::pattern::visitor::BaseVisitor,
      public llgc::pattern::visitor::Visitor<Number_Constant>,
      public llgc::pattern::visitor::Visitor<Number_NumOpNum>
{
 public:
  NumberVisitorVal() = default;
#ifndef SWIG
  NumberVisitorVal(NumberVisitorVal &&other) = delete;
  NumberVisitorVal(NumberVisitorVal const &other) = delete;
  NumberVisitorVal &operator=(NumberVisitorVal &&other) = delete;
  NumberVisitorVal &operator=(NumberVisitorVal const &other) = delete;
#endif  // !SWIG
  ~NumberVisitorVal() override = default;

  bool Visit(const Number_Constant &data,
             std::string *return_value) const override CHK;
  bool Visit(const Number_NumOpNum &data,
             std::string *return_value) const override CHK;
};

}  // namespace llgc::poco

#endif  // POCO_NUMBER_VISITOR_VALUE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
