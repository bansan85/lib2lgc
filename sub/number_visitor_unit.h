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

#ifndef NUMBER_VISITOR_UNIT_H_
#define NUMBER_VISITOR_UNIT_H_

#include "config.h"

// C++ system
#include <string>

// lib2lgcPoco
#include <number.pb.h>

// lib2lgcVisitor
#include <visitor.h>

namespace pattern {
namespace visitor {

// We can't include number.h because number.h need number_visitor_unit.h.
class Number_Constant;
class Number_NumOpNum;

/**
 * @brief The visitor that will return the unit.
 */
class NumberVisitorUnit : public BaseVisitor,
                          public Visitor<Number_Constant>,
                          public Visitor<Number_NumOpNum> {
 public:
  /**
   * @brief Default destructor.
   */
  virtual ~NumberVisitorUnit() {}
  /**
   * @brief Return the unit of a constant number.
   *
   * @param data The constant number.
   * @param return_value This parameter will be changed only if Visit success.
   *
   * @return true if success, false instead.
   */
  bool Visit(const Number_Constant &data,
             std::string *return_value) const override CHK;
  /**
   * @brief Return the unit of a number based on an operation of two numbers.
   *
   * @param data The number.
   * @param return_value This parameter will be changed only if Visit success.
   *
   * @return true if success, false instead.
   */
  bool Visit(const Number_NumOpNum &data,
             std::string *return_value) const override CHK;
};

}  // namespace visitor
}  // namespace pattern

#endif  // NUMBER_VISITOR_UNIT_H_
