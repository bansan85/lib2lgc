/* Copyright [2017] LE GARREC Vincent
 *
 * This file is part of 2LGC.
 *
 * 2LGC is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * 2LGC is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with 2LGC. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VISITABLE_H_
#define VISITABLE_H_

#include "config.h"

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
namespace pattern {
namespace visitor {

/**
 * @brief Common class to allow a class to be visited. This class is used to
 *        force the implementation of the Accept method with the right
 *        arguments.
 *
 * @tparam Visitable The same name of the class that is visitable.
 */
template <class Visitable, class T>
class BaseVisitable : virtual T {
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
              std::string *return_value) const override CHK {
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
class InterfaceVisitable {
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
