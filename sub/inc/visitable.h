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

#ifndef VISITABLE_H_
#define VISITABLE_H_

// C++ system
#include <string>

// Google protobuf library
#include <google/protobuf/message.h>

// Current project
#include "visitor.h"

/**
 * @file
 *
 * This header is used has base for every class that a visitor would like to
 * visit.
 */
namespace pattern
{
namespace visitor
{
/**
 * @brief Common class to allow a class to be visited. This class is used to
 *        force the implementation of the Accept method with the right
 *        arguments.
 *
 * @tparam Visitable The same name of the class that is visitable.
 */
template <class Visitable, class T>
class BaseVisitable : virtual T
{
 public:
  /**
   * @brief In case of some class based on it need a virtual destructor.
   */
  virtual ~BaseVisitable() {}

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
  static_assert(std::is_base_of<::google::protobuf::Message, M>::value,
                "M must be a descendant of ::google::protobuf::Message");

 public:
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
  const M &message() const { return message_; }

 protected:
  /**
   * @brief return a modifiable reference of the data so the visitor may modify
   * it.
   *
   * @return The message in read-write format.
   */
  M &message() { return message_; }

 private:
  /**
   * @brief The raw data.
   */
  M message_;
};

}  // namespace visitor
}  // namespace pattern

#endif  // VISITABLE_H_
