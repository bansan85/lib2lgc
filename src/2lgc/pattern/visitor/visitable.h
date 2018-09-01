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

#ifndef PATTERN_VISITOR_VISITABLE_H_
#define PATTERN_VISITOR_VISITABLE_H_

#include <2lgc/pattern/visitor/visitor.h>
#include <google/protobuf/message.h>
#include <string>

namespace llgc::pattern::visitor
{
template <class Visitable, class T>
class BaseVisitable : virtual public T
{
 public:
  BaseVisitable() = default;
#ifndef SWIG
  BaseVisitable(BaseVisitable &&other) = delete;
  BaseVisitable(BaseVisitable const &other) = delete;
  BaseVisitable &operator=(BaseVisitable &&other) = delete;
  BaseVisitable &operator=(BaseVisitable const &other) = delete;
#endif  // !SWIG
  ~BaseVisitable() override = default;

  bool Accept(const BaseVisitor &visitor,
              std::string *return_value) const override CHK
  {
    return dynamic_cast<const Visitor<Visitable> &>(visitor).Visit(
        dynamic_cast<const Visitable &>(*this), return_value);
  }
};

template <class M>
class InterfaceVisitable
{
  static_assert(std::is_base_of<::google::protobuf::Message, M>::value,
                "M must be a descendant of ::google::protobuf::Message.");

 public:
  InterfaceVisitable() : message_() {}
#ifndef SWIG
  InterfaceVisitable(InterfaceVisitable &&other) = delete;
  InterfaceVisitable(InterfaceVisitable const &other) = delete;
  InterfaceVisitable &operator=(InterfaceVisitable &&other) = delete;
  InterfaceVisitable &operator=(InterfaceVisitable const &other) = delete;
#endif  // !SWIG
  virtual ~InterfaceVisitable() = default;

  virtual bool Accept(const BaseVisitor &visitor,
                      std::string *return_value) const CHK = 0;
  const M &Message() const { return message_; }

 protected:
  M &Message() { return message_; }

 private:
  M message_;
};

}  // namespace llgc::pattern::visitor

#endif  // PATTERN_VISITOR_VISITABLE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
