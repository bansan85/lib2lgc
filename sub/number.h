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

#ifndef NUMBER_H_
#define NUMBER_H_

#include "config.h"

// C++ system
#include <string>

// lib2lgcPoco
#include <number.pb.h>

// lib2lgcVisitor
#include <visitable.h>
#include <visitor.h>

// Current project
#include "number_visitor_unite.h"
#include "number_visitor_value.h"

namespace pattern {
namespace visitor {

class Unite {
 public:
  static bool UniteOp(const msg::Number_Unite &unite1,
                      const msg::Number_Unite &unite2,
                      const msg::Number_Operator operator_,
                      msg::Number_Unite *return_unite) CHK;
};

class Number : public InterfaceVisitable<msg::Number> {
 public:
  Number()
#ifdef ENABLE_VISITABLE_CACHE
      : cache_value_(0.),
        cache_unite_()
#endif  // ENABLE_VISITABLE_CACHE
  {
  }
  virtual ~Number() {}

  virtual double GetVal() const = 0;
  virtual msg::Number_Unite GetUnite() const = 0;

 protected:
  static pattern::visitor::NumberVisitorVal visitor_val;
  static pattern::visitor::NumberVisitorUnite visitor_unite;

#ifdef ENABLE_VISITABLE_CACHE
  mutable double cache_value_;
  mutable msg::Number_Unite cache_unite_;
#endif  // ENABLE_VISITABLE_CACHE
};

class Number_Constant : virtual public Number,
                        virtual public BaseVisitable<Number_Constant, Number> {
 public:
  Number_Constant(const uint32_t id, const double value,
                  msg::Number_Unite *unite);
  virtual ~Number_Constant() {}

  double GetVal() const override;
  msg::Number_Unite GetUnite() const override;

 private:
#ifdef ENABLE_VISITABLE_CACHE
  mutable unsigned int cache_value_id_;
  mutable unsigned int cache_unite_id_;
#endif  // ENABLE_VISITABLE_CACHE
};

class Number_NumOpNum : virtual public Number,
                        virtual public BaseVisitable<Number_NumOpNum, Number> {
 public:
  Number_NumOpNum(const uint32_t id, const Number &number1,
                  msg::Number_Operator operator_, const Number &number2);
  virtual ~Number_NumOpNum() {}

  double GetVal() const override;
  msg::Number_Unite GetUnite() const override;

  const Number &number1() const { return number1_; }
  const Number &number2() const { return number2_; }

 private:
  const Number &number1_;
  const Number &number2_;

#ifdef ENABLE_VISITABLE_CACHE
  mutable unsigned int cache_value1_id_;
  mutable unsigned int cache_value2_id_;
  mutable unsigned int cache_unite1_id_;
  mutable unsigned int cache_unite2_id_;
#endif  // ENABLE_VISITABLE_CACHE
};

}  // namespace visitor
}  // namespace pattern

#endif  // NUMBER_H_
