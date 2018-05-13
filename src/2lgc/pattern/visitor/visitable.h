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

/**
 * @file
 *
 * This header is used has base for every class that a visitor would like to
 * visit.
 */
START_NAMESPACE3(llgc, pattern, visitor)

/**
 * @brief Common class to allow a class to be visited. This class is used to
 *        force the implementation of the Accept method with the right
 *        arguments.
 *
 * @tparam Visitable The same name of the class that is visitable.
 */
template <class Visitable, class T>
class BaseVisitable : virtual public T
{
 public:
  /**
   * @brief In case of some class based on it need a virtual destructor.
   */
  ~BaseVisitable() override {}

  /**
   * @brief This method is used to call the right Visit method on the visitor.
   *
   * @param visitor The visitor that will used the data of the class that
   *        implement this class.
   * @param return_value The return value from the visitor from
   *        SerializeToString.
   *
   * @return false if there is a problem or if SerializeToString failed.
   *         true instead.
   */
  bool Accept(const BaseVisitor &visitor,
              std::string *return_value) const override CHK
  {
    return dynamic_cast<const Visitor<Visitable> &>(visitor).Visit(
        dynamic_cast<const Visitable &>(*this), return_value);
  }
};

/**
 * @brief Common class to implement interface visitable. This class only
 *        have the pure virtual methods Accept.
 *
 * @tparam M The class that contains the data. It must be a ProtoBuf message.
 */
template <class M>
class InterfaceVisitable
{
  static_assert(std::is_base_of<::google::protobuf::Message, M>::value,  // NS
                "M must be a descendant of ::google::protobuf::Message.");

 public:
  /**
   * @brief Default constructor.
   */
  InterfaceVisitable() : message_() {}
  /**
   * @brief In case of some class based on it need a virtual destructor.
   */
  virtual ~InterfaceVisitable() {}

  /**
   * @brief This method is used to call the right Visit method on the visitor.
   *
   * @param visitor The visitor that will used the data of the class that
   *        implement this class.
   * @param return_value The return value from the visitor from
   *        SerializeToString. The value passed must be allocated on the stack
   *        or on the pile.
   *
   * @return false if there is a problem or if SerializeToString failed.
   *         true instead.
   */
  virtual bool Accept(const BaseVisitor &visitor,
                      std::string *return_value) const CHK = 0;

  /**
   * @brief return a const reference of the data so the visitor may use it.
   *
   * @return The message in read-only format.
   */
  const M &Message() const { return message_; }

 protected:
  /**
   * @brief return a modifiable reference of the data so the visitor may modify
   * it.
   *
   * @return The message in read-write format.
   */
  M &Message() { return message_; }

 private:
  /**
   * @brief The raw data.
   */
  M message_;
};

END_NAMESPACE3(llgc, pattern, visitor)

#endif  // PATTERN_VISITOR_VISITABLE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
