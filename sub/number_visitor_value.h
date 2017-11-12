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

#ifndef NUMBER_VISITOR_VALUE_H_
#define NUMBER_VISITOR_VALUE_H_

#include "config.h"

// C++ system
#include <string>

// lib2lgcPoco
#include <number.pb.h>

// lib2lgcVisitor
#include <visitor.h>

namespace pattern {
namespace visitor {

// We can't include number.h because number.h need number_visitor_val.h.
class Number_Constant;
class Number_NumOpNum;

class NumberVisitorVal : public BaseVisitor,
                         public Visitor<Number_Constant>,
                         public Visitor<Number_NumOpNum> {
 public:
  virtual ~NumberVisitorVal() {}
  bool Visit(const Number_Constant &data,
             std::string *return_value) const override CHK;
  bool Visit(const Number_NumOpNum &data,
             std::string *return_value) const override CHK;
};

}  // namespace visitor
}  // namespace pattern

#endif  // NUMBER_VISITOR_VALUE_H_
