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

#ifndef PATTERN_VISITOR_VISITOR_H_
#define PATTERN_VISITOR_VISITOR_H_

#include <2lgc/compat.h>
#include <string>

namespace llgc::pattern::visitor
{
class BaseVisitor
{
 public:
  BaseVisitor() = default;
  virtual ~BaseVisitor() = default;

#ifndef SWIG
  BaseVisitor(BaseVisitor &&other) = delete;
  BaseVisitor(BaseVisitor const &other) = default;
  BaseVisitor &operator=(BaseVisitor &&other) = delete;
  BaseVisitor &operator=(BaseVisitor const &other) = delete;
#endif  // !SWIG
};

template <class T>
class Visitor
{
 public:
  Visitor() = default;
  virtual ~Visitor() = default;

#ifndef SWIG
  Visitor(Visitor &&other) = delete;
  Visitor(Visitor const &other) = delete;
  Visitor &operator=(Visitor &&other) = delete;
  Visitor &operator=(Visitor const &other) = delete;
#endif  // !SWIG

  virtual bool Visit(const T &data, std::string *return_value) const CHK = 0;
};

}  // namespace llgc::pattern::visitor

#endif  // PATTERN_VISITOR_VISITOR_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
